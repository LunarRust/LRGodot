#ifndef EMISSION_POINT_GENERATOR_H
#define EMISSION_POINT_GENERATOR_H

#include "core/object/ref_counted.h"
#include "scene/3d/gpu_particles_3d.h"

class EmissionPointGenerator : public RefCounted {
	GDCLASS(EmissionPointGenerator, RefCounted);

	GPUParticles3D *node = nullptr;

public:
	enum EmissionSources {
		SURFACE_POINTS,
		SURFACE_POINTS_AND_NORMAL_DIRECTED,
		VOLUME
	};

	void select_particles(const NodePath &p_path);
    void select_mesh(const NodePath &p_path);
    void generate_emission_points(int emission_source, int emission_count);


protected:
	static void _bind_methods();

	Node3D *base_node = nullptr;
	
    Vector<Face3> geometry;

	bool _generate(Vector<Vector3> &points, Vector<Vector3> &normals, int emission_source, int emission_count);
    


	EmissionPointGenerator();
};

VARIANT_ENUM_CAST(EmissionPointGenerator::EmissionSources);

#endif // EMISSION_POINT_GENERATOR_H
