#include "lunaruststates.h"
#include "core/object/class_db.h"
#include "scene/main/scene_tree.h"


void LunarustStates::_bind_methods() {
	ClassDB::bind_method(D_METHOD("ChangePlayerMode", "mode"), &LunarustStates::ChangePlayerMode);
	ClassDB::bind_method(D_METHOD("get_player_mode"), &LunarustStates::get_player_mode);
	//Enums
	BIND_ENUM_CONSTANT(fps);
	BIND_ENUM_CONSTANT(vn);
	BIND_ENUM_CONSTANT(investigation);
	BIND_ENUM_CONSTANT(psycho);
	BIND_ENUM_CONSTANT(dead);
	//Properties
	ADD_PROPERTY(PropertyInfo(Variant::INT, "player_mode", PROPERTY_HINT_ENUM, "fps,vn,investigation,psycho,dead"), "ChangePlayerMode", "player_mode");
	ADD_SIGNAL(MethodInfo("player_mode_changed", PropertyInfo(Variant::INT, "mode")));
}
void LunarustStates::ChangePlayerMode(PlayerModes mode) {
	player_mode = mode;
	emit_signal("player_mode_changed", player_mode);
}
LunarustStates::PlayerModes LunarustStates::get_player_mode() {
	return player_mode;
}

LunarustStates::LunarustStates() {
	player_mode = PlayerModes::fps;
}
