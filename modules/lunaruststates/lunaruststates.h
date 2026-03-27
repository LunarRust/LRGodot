#ifndef LUNARUSTSTATES_H
#define LUNARUSTSTATES_H

#include "core/object/ref_counted.h"
#include "core/variant/type_info.h" // Required for VARIANT_ENUM_CAST use

class LunarustStates : public RefCounted {
	GDCLASS(LunarustStates, RefCounted);

public:
	
	enum PlayerModes : int {
		fps,
		vn,
		investigation,
		psycho,
		dead
	};

	PlayerModes player_mode;


protected:
	static void _bind_methods();

public:
	
	void ChangePlayerMode(PlayerModes mode);
	PlayerModes get_player_mode();

	LunarustStates();
};
VARIANT_ENUM_CAST(LunarustStates::PlayerModes);

#endif // LUNARUSTSTATES_H
