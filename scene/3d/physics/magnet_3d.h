#include "core/object/class_db.h"
#include "scene/main/scene_tree.h"
#include "scene/3d/physics/area_3d.h"

class Magnet3D : public Area3D {
    GDCLASS(Magent3D, Area3D);


public:
    Node3D target;
    float strength;
    float damping_strength;

protected:
	static void _bind_methods();

}