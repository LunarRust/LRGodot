#ifndef LUNARUSTSTATES_H
#define LUNARUSTSTATES_H

#include "core/object/ref_counted.h"



class LunarustStates : public RefCounted {
	GDCLASS(LunarustStates, RefCounted);
	
	Signal player_mode_changed;


public:
	
	enum PlayerModes : int {
		fps,
		vn,
		investigation,
		psycho
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
