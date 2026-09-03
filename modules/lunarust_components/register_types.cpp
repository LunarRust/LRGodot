#include "register_types.h"

#include "core/object/class_db.h"
#include "lunarust_character_body_3d.h"

void initialize_lunarust_components_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<LunarustCharacterBody3D>();
	
}

void uninitialize_lunarust_components_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
   // Nothing to do here in this example.
}
