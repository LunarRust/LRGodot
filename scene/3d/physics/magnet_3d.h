//
// intial GDscript implementation: https://github.com/Dilonk1234
// 
// Engine implementation: Juniper Mitchell
//

#pragma once
#include "core/object/class_db.h"
#include "scene/3d/physics/area_3d.h"


class Magnet3D : public Area3D {
	GDCLASS(Magnet3D, Area3D);

public:
	// Exposed as a NodePath property to be robust when the node isn't in the tree yet.
	NodePath target_path;
	float strength = 20.0f;
	float damping_strength = 5.0f;

	Magnet3D(); // <- add constructor declaration

	void set_target_path(const NodePath &p_path);
	NodePath get_target_path() const;

	void set_strength(float p_strength);
	float get_strength() const;

	void set_damping_strength(float p_damping);
	float get_damping_strength() const;

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);
};
