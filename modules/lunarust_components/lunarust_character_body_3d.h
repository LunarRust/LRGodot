#include "scene/3d/physics/character_body_3d.h"
#include "core/variant/type_info.h" // Required for VARIANT_ENUM_CAST use

class LunarustCharacterBody3D : public CharacterBody3D {
	GDCLASS(LunarustCharacterBody3D,CharacterBody3D);

protected:
	static void _bind_methods();

public:

	bool move_and_collide_own(float delta,Variant stats);

};
