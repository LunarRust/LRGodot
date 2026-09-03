#pragma once
#include "core/io/resource.h"
class PlayerVariables : public Resource {
	GDCLASS(PlayerVariables, Resource);

protected:
	static void _bind_methods();

public:
	float ply_mousesensitivity = 1.5f;
	float ply_maxlookangle_down = -90.0f;
	float ply_maxlookangle_up = 90.0f;
	float ply_ylookspeed = 0.3f;
	float ply_xlookspeed = 0.3f;
	float ply_sidespeed = 20.0f;
	float ply_forwardspeed = 20.0f;
	float ply_backspeed = 20.0f;
	float ply_upspeed = 20.0f;

	float ply_acceleration = 7.0f;
	float ply_deceleration = 7.0f;
	float ply_airacceleration = 400.0f;
	float ply_maxacceleration = 10.0f;
	float ply_airspeedcap = 1.0f;
	float ply_friction = 3.0f;
	float ply_stopspeed = 50.0f;
	float ply_gravity = 9.8f;
	float ply_slopeangle_max = 45.0f;
	float ply_maxvelocity = 100.0f;

	float ply_jumpheight = 4.0f;
	int ply_stepsize = 8;

	float ply_maxspeed = 16.0f;


private:
	//private members and methods
	float speed = 16.0f;




}
