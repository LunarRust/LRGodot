/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "emission_point_generator.h"

void initialize_emission_point_generator_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
	GDREGISTER_CLASS(EmissionPointGenerator);
}

void preregister_emission_point_generator_types() {
	// Called before any other core types are registered.
	// Nothing to do here in this example.
}

void register_emission_point_generator_types() {
	GDREGISTER_CLASS(EmissionPointGenerator);
}

void unregister_emission_point_generator_types() {
	// Nothing to do here in this example.
}

void uninitialize_emission_point_generator_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
   // Nothing to do here in this example.
}
