#include "scene/3d/physics/magnet_3d.h"

#include "core/object/class_db.h"
#include "scene/main/scene_tree.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "servers/physics_3d/physics_server_3d.h"

// Magnet3D:
// - An Area3D-derived helper that applies attraction forces to overlapping PhysicsBody3D nodes.
// - Properties are exposed via ClassDB in _bind_methods so they appear in the editor and can be serialized.

// Constructor: Enable physics notifications so NOTIFICATION_PHYSICS_PROCESS is delivered.
Magnet3D::Magnet3D() {
	set_physics_process(true);

	// Don't call set_monitoring/set_monitorable here — the Area may not have a valid RID yet.
	ERR_PRINT_ONCE("Magnet3D: constructed (physics process enabled).");
}

/* ---- Property setters/getters ------------------------------------------------ */

void Magnet3D::set_target_path(const NodePath &p_path) {
	// Store a NodePath instead of a raw pointer. This keeps the resource robust across
	// scene load/unload and avoids dangling pointers when nodes are reparented.
	target_path = p_path;
}

NodePath Magnet3D::get_target_path() const {
	return target_path;
}

void Magnet3D::set_strength(float p_strength) {
	strength = p_strength;
}

float Magnet3D::get_strength() const {
	return strength;
}

void Magnet3D::set_damping_strength(float p_damping) {
	damping_strength = p_damping;
}

float Magnet3D::get_damping_strength() const {
	return damping_strength;
}

/* ---- ClassDB bindings (editor + scripting) ---------------------------------- */

void Magnet3D::_bind_methods() {
	// Expose the target NodePath property to the editor and scripting.
	// Variant::NODE_PATH identifies the property type; PROPERTY_HINT_NODE_PATH_VALID_TYPES
	// restricts the path picker to Node3D types in the editor.
	ClassDB::bind_method(D_METHOD("set_target_path", "path"), &Magnet3D::set_target_path);
	ClassDB::bind_method(D_METHOD("get_target_path"), &Magnet3D::get_target_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node3D"), "set_target_path", "get_target_path");

	// Expose numeric tuning properties.
	ClassDB::bind_method(D_METHOD("set_strength", "strength"), &Magnet3D::set_strength);
	ClassDB::bind_method(D_METHOD("get_strength"), &Magnet3D::get_strength);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "strength", PROPERTY_HINT_NONE, "N/m"), "set_strength", "get_strength");

	ClassDB::bind_method(D_METHOD("set_damping_strength", "damping_strength"), &Magnet3D::set_damping_strength);
	ClassDB::bind_method(D_METHOD("get_damping_strength"), &Magnet3D::get_damping_strength);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping_strength"), "set_damping_strength", "get_damping_strength");
}

/* ---- Runtime behavior (physics tick) --------------------------------------- */

void Magnet3D::_notification(int p_what) {
	// When node enters the physics world, enable monitoring so the physics server
	// registers the Area and get_overlapping_bodies() returns results.
	if (p_what == NOTIFICATION_ENTER_WORLD) {
		set_monitoring(true);
		set_monitorable(true);
	}

	// Disable monitoring when leaving the physics world to avoid stale registrations.
	if (p_what == NOTIFICATION_EXIT_WORLD) {
		set_monitoring(false);
		set_monitorable(false);
	}

	// Only run during the physics step. NOTIFICATION_PHYSICS_PROCESS is sent at the engine's
	// fixed physics tick rate. Using it ensures deterministic physics interactions.
	if (p_what != NOTIFICATION_PHYSICS_PROCESS) {
		return;
	}

	// Resolve target node robustly:
	// - Avoid direct casting from Variant (some engine helper overloads return Variant).
	// - Use has_node/get_node to retrieve a Node* if present, then cast to Node3D.
	Node *node_raw = nullptr;
	if (!target_path.is_empty() && has_node(target_path)) {
		node_raw = get_node(target_path);
	}
	Node3D *target_node = Object::cast_to<Node3D>(node_raw);
	if (target_node == nullptr) {
		// ERR_PRINT_ONCE avoids spamming the log if the target is missing for many ticks.
		ERR_PRINT_ONCE("Magnet3D: target is not set or not a Node3D. Magnet will do nothing.");
		return;
	}

	// Area3D::get_overlapping_bodies() returns a TypedArray<Node3D> of instances currently
	// overlapping this area. We iterate it and operate only on physics bodies.
	TypedArray<Node3D> bodies = get_overlapping_bodies();

	// Diagnostic: print count so you can confirm overlaps are detected.

	for (int i = 0; i < bodies.size(); ++i) {
		// `TypedArray` elements are stored as Variant underneath. Avoid direct initialization
		// to `Node3D*` from Variant (which can trigger the conversion error).
		// Instead, obtain the underlying Object* then cast safely to Node3D.
		Object *obj = bodies[i];
		if (!obj) {
			continue; // safety, though get_overlapping_bodies normally filters nulls
		}

		Node3D *node = Object::cast_to<Node3D>(obj);
		if (!node) {
			continue;
		}

		// Cast to the abstract PhysicsBody3D base so we work with any physics backend/type.
		// Concrete backends (Godot/Jolt) implement the actual force application.
		PhysicsBody3D *pb = Object::cast_to<PhysicsBody3D>(node);
		if (!pb) {
			// Not a physics body (could be a plain Node3D), skip.
			continue;
		}

		// Compute vector from body to the configured target.
		const Vector3 body_pos = pb->get_global_transform().origin;
		const Vector3 target_pos = target_node->get_global_transform().origin;
		Vector3 direction = target_pos - body_pos;
		const float distance = direction.length();

		// CMP_EPSILON is the engine's small epsilon constant to avoid divide-by-zero and jitter.
		if (distance <= CMP_EPSILON) {
			// Body already at the exact target position — skip applying force.
			continue;
		}

		// Damping: simple proportional damping based on the body's linear velocity.
		// Negative sign resists the current motion so the magnet can "pull and settle" bodies.
		Vector3 damping = -pb->get_linear_velocity() * damping_strength;

		// Force falloff: here we use an inverse-square-like falloff. This is just an example;
		// you can change it to linear, inverse, clamped, or any custom formula.
		Vector3 force = direction * strength;

		// Apply the computed force via the PhysicsServer API using the body's RID.
		// Using PhysicsServer3D ensures compatibility with different physics modules
		// (Godot's built-in, Jolt, etc.) and lets the server handle wake-up semantics.
		real_t dt = 0;
		if (get_tree()) {
			dt = get_tree()->get_physics_process_time();
		}

		RID body_rid = pb->get_rid();
		PhysicsServer3D::get_singleton()->body_apply_central_force(body_rid, (force) + damping);
	}
}
