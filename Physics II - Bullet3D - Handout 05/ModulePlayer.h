#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 400
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	bool victory = false;
	PhysBody3D* object;
	Cube* mushrooms[3] = { nullptr };

	int numboosts = 3;

	int accelerateTimer = 70;
	int deceleration = 70;
	bool deaccelerated = false;
	bool boosting = false;

	int coins = 0;
	int extraAcceleration = 0;
	vec3 cameraPt;

	btVector3 myDrag;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
};