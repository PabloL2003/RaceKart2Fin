#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "Timer.h"

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
	void Teleport();
	void ChangeFriction(float friction);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	bool debuggingFriction = false;
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
	btVector3 myLift;

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float frictionCoefficient;

	int coinCollectMinutes;
	int coinCollectSeconds;
	int coinCollectMilliseconds;
	bool timerRunning;
	Uint32 startTime;

	float vehicleArea;

	Timer SpawningTimer;

	vec3 teleportPos;
	mat4x4 teleportTransform;

	uint congratulation;
	uint welcome;
	uint yipee;

	float mass = 700.0f;
};