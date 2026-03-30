#include "core/object/class_db.h"
#include "scene/main/scene_tree.h"
#include "scene/3d/physics/area_3d.h"
#include "scene/3d/physics/magnet_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "modules/jolt_physics/objects/jolt_body_3d.h"


void _bind_methods() {
    ADD_PROPERTY(PropertyInfo(Variant::Node3D,"target",PROPERTY_HINT_NODE_PATH_VALID_TYPES))
}


void _notification() {
    if (target == nullptr){
        ERR_FAIL_COND_V_MSG("Magnet3D Needs a target. Magnet will do nothing");
        return;
    }
    
    for (int i = 0; i < Area3D.get_overlapping_bodies(); i++){
        if Area3D.get_overlapping_bodies()[i] == JoltBody3D {
            Vector3 direction = target.global_transform.position - Area3D.get_overlapping_bodies()[i].global_transform.position;
            Vector3 distance = direction.p_length;
            Vector3 damping = -Area3D.get_overlapping_bodies()[i].get_linear_velocity() * damping_strength;
            if (distance > 0.0f) {
                Vector3 force = direction * strength;
                i.apply_central_force(force + damping);
            }
        }
    }

}