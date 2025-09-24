#include "register_types.h"

#include "core/object/class_db.h"
#include "lunaruststates.h"

void initialize_lunaruststates_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<LunarustStates>();
	
}

void uninitialize_lunaruststates_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
   // Nothing to do here in this example.
}
