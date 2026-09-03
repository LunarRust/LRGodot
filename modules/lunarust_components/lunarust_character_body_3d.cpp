#include "lunarust_character_body_3d.h"
#include "scene/3d/physics/character_body_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "servers/physics_3d/physics_server_3d.h"

#include "core/object/class_db.h"
#include "scene/main/scene_tree.h"


void LunarustCharacterBody3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("move_and_collide_own", "delta", "stats"), &LunarustCharacterBody3D::move_and_collide_own);
}

bool LunarustCharacterBody3D::move_and_collide_own(float delta, Variant stats) {
	bool collided = false;
	//stats.on_floor = false;

	//check floor
	Vector3 checkMotion = get_velocity() * (1 / 60);

	return false;
	//checkMotion.y -= stats.ply_gravity * (1/360)

}
