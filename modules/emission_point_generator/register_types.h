/* register_types.h */

#ifndef EMISSION_POINT_GENERATOR_REGISTER_TYPES_H
#define EMISSION_POINT_GENERATOR_REGISTER_TYPES_H

#include "modules/register_module_types.h"

void initialize_emission_point_generator_module(ModuleInitializationLevel p_level);
void uninitialize_emission_point_generator_module(ModuleInitializationLevel p_level);
/* yes, the word in the middle must be the same as the module folder name */

#define MODULE_EMISSION_POINT_GENERATOR_HAS_PREREGISTER
void preregister_emission_point_generator_types();

void register_emission_point_generator_types();
void unregister_emission_point_generator_types();

#endif
