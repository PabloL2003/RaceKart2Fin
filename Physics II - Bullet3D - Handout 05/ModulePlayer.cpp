#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "SDL/include/SDL.h"

#define COIN_REQUIREMENT 10

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 700.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(-26, 1.5f, 10);
	float orbDistance = 4.0f;
	SpawningTimer.Start();

	teleportPos = { -26,1.5f,10 };

	for (int i = 0; i < 3; ++i) {
		mushrooms[i] = new Cube(1, 1, 1);
		
		object = App->physics->AddBody(Cube(1, 1, 1), 1);
		object->SetPos( i* 23 , 20, 10 + orbDistance);
		mushrooms[i]->pbody = object;
		App->physics->AddConstraintHinge(*vehicle, *object, vec3(0, 2, 0), vec3(0, 0, -orbDistance), vec3(0, 1, 0), vec3(0, 1, 0), true);
		object->SetAsSensor(true);
	}

	coinCollectMinutes = 0;
	coinCollectSeconds = 0;
	coinCollectMilliseconds = 0;
	timerRunning = false;

	teleportTransform[0] = 1;
	teleportTransform[1] = 0;
	teleportTransform[2] = 0;
	
	teleportTransform[4] = 0;
	teleportTransform[5] = 1;
	teleportTransform[6] = 0;
	
	teleportTransform[8] = 0;
	teleportTransform[9] = 0;
	teleportTransform[10] = 1;

	frictionCoefficient = 0.0f;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	////delete mushrooms
	//for (int i = 0; i < 3; ++i) {
	//	delete mushrooms[i];
	//	mushrooms[i] = nullptr;
	//}

	////volver a crear los mushrooms
	//float orbDistance = 4.0f;
	//for (int i = 0; i < 3; ++i) {
	//	mushrooms[i] = new Cube(1, 1, 1);

	//	object = App->physics->AddBody(Cube(1, 1, 1), 1);
	//	object->SetPos(i * 23, 20, 10 + orbDistance);
	//	mushrooms[i]->pbody = object;
	//	App->physics->AddConstraintHinge(*vehicle, *object, vec3(0, 2, 0), vec3(0, 0, -orbDistance), vec3(0, 1, 0), vec3(0, 1, 0), true);
	//	object->SetAsSensor(true);
	//}
	numboosts = 3;

	coins = 0;

	coinCollectMinutes = 0;
	coinCollectSeconds = 0;
	coinCollectMilliseconds = 0;

	//resetting vehicle position and velocity
	vehicle->SetPos(-26, 1.5f, 10);
	vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
	vehicle->body->setLinearVelocity(btVector3(0, 0, 0));

	btVector3 a = { 0,0,0 };
	vehicle->SetTransform(teleportTransform.M);
	vehicle->body->setAngularVelocity(a);
	vehicle->body->setLinearVelocity(a);
	vehicle->SetPos(teleportPos.x, teleportPos.y, teleportPos.z);

	return true;
}

btVector3 MathVecToBt2(vec3 v) {

	return btVector3(v.x, v.y, v.z);
}


vec3 btVecToMath2(btVector3 v) {

	return vec3(v.x(), v.y(), v.z());
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (SpawningTimer.Read() < 700) {
		vehicle->body->setAngularVelocity( btVector3(0, 0, 0));
	}


	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION + extraAcceleration;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		/*brake = BRAKE_POWER;*/
		acceleration += -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && numboosts > 0) {
		if(!boosting){ numboosts--; }
		boosting = true;
		
		
	}
	else if (!deaccelerated) {
		vehicle->ApplyEngineForce(acceleration);
	}

	if (coins >= 5) {

		App->restartGame = true;
		
		if (timerRunning)
		{
			timerRunning = false;
		}
		//restart game
		
		

	}
	else
	{
		if (!timerRunning && coins >= 0) {
			timerRunning = true;
			startTime = SDL_GetTicks();	
		}

		if (timerRunning) {
			Uint32 currentTime = SDL_GetTicks();
			Uint32 elapsedTime = currentTime - startTime;

			coinCollectMinutes = elapsedTime / (1000 * 60);
			coinCollectSeconds = (elapsedTime / 1000) % 60;
			coinCollectMilliseconds = elapsedTime % 1000;
		}
	}

	// Boosting power logic

	if (boosting) {
		accelerateTimer--;
		vehicle->body->setAngularVelocity(btVector3(0,0,0));
		vehicle->ApplyEngineForce(MAX_ACCELERATION * 17);
		if (accelerateTimer <= 0) {
			boosting = false;
			accelerateTimer = 70;
			deaccelerated = true;
		}
	}

	if (deaccelerated) {
		deceleration--;
		
		vehicle->ApplyEngineForce(-MAX_ACCELERATION *10 );  // Applying negative force for deacceleration
		if (deceleration <= 0) {
			deaccelerated = false;
			deceleration = 70;
		}
	}

	// To reset player position if turned
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		btVector3 p = vehicle->GetPosition();
		vehicle->ResetCarOrientation(0);
		vehicle->SetPos(p.x(), p.y(), p.z());

	}

	if (vehicle->GetPosition().getY() <= 0.0f) {
		Teleport();
	}

	if (vehicle->GetPosition().getY() < 0.5f && !debuggingFriction) {
		frictionCoefficient = 1.0f;
		ChangeFriction(frictionCoefficient);
	}
	else if(!debuggingFriction)
	{
		frictionCoefficient = 5000.0f;
		ChangeFriction(frictionCoefficient);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {

		if (debuggingFriction) { debuggingFriction = false; }
		else {

			debuggingFriction = true;

			frictionCoefficient = 10000;
			ChangeFriction(10000);
		}
	}
	
	

	
	btVector3 vehicleForwardVector = vehicle->GetForwardVector();
	btVector3 cameraPt = vehicle->GetPosition() - vehicleForwardVector * 10.0f + btVector3(0.0f, 5.0f, 0.0f);

	App->camera->Look(btVecToMath2( cameraPt), btVecToMath2( vehicle->GetPosition()), true);


	
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	

	vehicle->Render();
	for (int i = 0; i < numboosts; ++i) {
		mushrooms[i]->Update(mushrooms[i]->pbody);
	}


	char title[170];
	sprintf_s(title, "%.1f Km/h   Coins: %d   DragForce = %f, ,%f , %f   Position = %f, %f, %f   Friction = %f    Timer: %02d:%02d:%03d", vehicle->GetKmh(), coins, myDrag.x() , myDrag.y() , myDrag.z(), vehicle->GetPosition().getX(), vehicle->GetPosition().getY(), vehicle->GetPosition().getZ(), frictionCoefficient, coinCollectMinutes, coinCollectSeconds, coinCollectMilliseconds);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::Teleport()
{
	btVector3 a = { 0,0,0 };
	vehicle->SetTransform(teleportTransform.M);
	vehicle->body->setAngularVelocity(a);
	vehicle->body->setLinearVelocity(a);
	vehicle->SetPos(teleportPos.x, teleportPos.y, teleportPos.z);
}

void ModulePlayer::ChangeFriction(float friction)
{
	vehicle->vehicle->m_wheelInfo[0].m_frictionSlip = friction;
	vehicle->vehicle->m_wheelInfo[1].m_frictionSlip = friction;
	vehicle->vehicle->m_wheelInfo[2].m_frictionSlip = friction;
	vehicle->vehicle->m_wheelInfo[3].m_frictionSlip = friction;
}


void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	LOG("Collision");
	if (body1->is_coin && body1->touched == false) {
		
		App->audio->PlayFx(App->scene_intro->coinFx);
		App->player->extraAcceleration += 50;
		App->player->coins++;
		body1->touched = true;

	}


};
