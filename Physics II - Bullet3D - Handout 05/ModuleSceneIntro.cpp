#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

void ModuleSceneIntro::addCube(vec3 pos, vec3 size, Color rgb, float rtx, float rty, float rtz,int id) {

	// id 5 = arena id 4 = pared id 2 = carretera//

	Cube cube;

	cube.SetPos(pos.x, pos.y, pos.z);
	cube.size = size;
	cube.color = rgb;

	if (rtx != 0)
		cube.SetRotation(rtx, { 1,0,0 });

	if (rty != 0)
		cube.SetRotation(rty, { 0,1,0 });

	if (rtz != 0)
		cube.SetRotation(rtz, { 0,0,1 });


	App->physics->AddBody(cube, 0);
	Borderblocks.add(cube);


}
// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//box = Cube(1000, 10, 10);
	//box.SetPos(1, 1, 1);
	//App->physics->AddBody(box, 50000.0f);

	coinFx = App->audio->LoadFx("Assets/coin.wav");


	//floor//
	addCube({ 0,0,0 }, { 200,0.2f,400 }, { 255,255,0 }, 0, 0, 0,5);


	//short wall//
	addCube({ -100,2,-200 }, { 50,6,6 }, { 0,255,0 }, 0, 0, 0,4);
	addCube({ -50,2,-200 }, { 50,6,6 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ 0,2,-200 }, { 50,6,6 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ 50,2,-200 }, { 50,6,6 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ 100,2,-200 }, { 50,6,6 }, { 0,0,255 }, 0, 0, 0, 4);


	//short wall//
	addCube({ -120,2,200 }, { 50,6,6 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ -70,2,200 }, { 50,6,6 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ -20,2,200 }, { 50,6,6 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ 30,2,200 }, { 50,6,6 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ 80,2,200 }, { 50,6,6 }, { 0,0,255 }, 0, 0, 0, 4);



	//long wall//
	addCube({ 100,2,-200 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ 100,2,-150 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ 100,2,-100 }, { 6,6,50 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ 100,2,-50 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ 100,2,0 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ 100,2,50 }, { 6,6,50 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ 100,2,100 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ 100,2,150 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ 100,2,187 }, { 6,6,25 }, { 0,255,0 }, 0, 0, 0, 4);


	//long wall//

	addCube({ -100,2,-200 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ -100,2,-150 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ -100,2,-100 }, { 6,6,50 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ -100,2,-50 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ -100,2,0 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ -100,2,50 }, { 6,6,50 }, { 0,255,0 }, 0, 0, 0, 4);
	addCube({ -100,2,100 }, { 6,6,50 }, { 0,0,255 }, 0, 0, 0, 4);
	addCube({ -100,2,150 }, { 6,6,50 }, { 255,0,0 }, 0, 0, 0, 4);
	addCube({ -100,2,187 }, { 6,6,25 }, { 0,255,0 }, 0, 0, 0, 4);



	//carretera//
	//Las curbas no estan coordenadas//
	// 
	//recta//
	addCube({ -85,0,0 }, { 10,0.3f,160 }, { 1,1,1 }, 0, 0, 0,2);

	//circuito//
	addCube({ -84,0,-84 }, { 6,0.3f,10 }, { 1,1,1 }, 0, -15, 0, 2);
	addCube({ -81,0,-91 }, { 6,0.3f,10 }, { 1,1,1 }, 0, -35, 0, 2);
	addCube({ -76,0,-96 }, { 6,0.3f,10 }, { 1,1,1 }, 0, -65, 0, 2);
	addCube({ -68,0,-97 }, { 6,0.3f,10 }, { 1,1,1 }, 0, -95, 0, 2);
	addCube({ -54,0,-86 }, { 6.5f,0.3f,30 }, { 1,1,1 }, 0, -135, 0, 2);
	addCube({ -40,0,-75 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -105, 0, 2);
	addCube({ -23,0,-73.5f }, { 6.5f,0.3f,30 }, { 1,1,1 }, 0, -90, 0, 2);
	addCube({ -4,0,-75 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -75, 0, 2);
	addCube({ 3,0,-79 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -45, 0, 2);
	addCube({ 9,0,-87 }, { 6.5f,0.3f,30 }, { 1,1,1 }, 0, -35, 0, 2);
	addCube({ 23,0,-102 }, { 6.5f,0.3f,15 }, { 1,1,1 }, 0, -65, 0, 2);
	addCube({ 38,0,-104 }, { 6.5f,0.3f,25 }, { 1,1,1 }, 0, -90, 0, 2);
	addCube({ 53,0,-102 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -120, 0, 2);
	addCube({ 58,0,-96 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -150, 0, 2);
	addCube({ 60,0,-88 }, { 6.5f,0.3f,10 }, { 1,1,1 }, 0, -175, 0, 2);
	addCube({ 18,0,-42.5f }, { 8,0.3f,120 }, { 1,1,1 }, 0, -225, 0, 2);
	addCube({ -25,0,3 }, { 10,0.3f,10 }, { 1,1,1 }, 0, -195, 0, 2);
	addCube({ -26,0,16 }, { 10,0.3f,25 }, { 1,1,1 }, 0, -180, 0, 2);
	addCube({ -24,0,30 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -155, 0, 2);
	addCube({ -20,0,35 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -120, 0, 2);
	addCube({ 12,0,36 }, { 9,0.3f,65 }, { 1,1,1 }, 0, -90, 0, 2);
	addCube({ 47,0,38 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -120, 0, 2);
	addCube({ 53.5f,0,45 }, { 9,0.3f,15 }, { 1,1,1 }, 0, -145, 0, 2);
	addCube({ 59,0,56 }, { 9,0.3f,15 }, { 1,1,1 }, 0, -160, 0, 2);
	addCube({ 61,0,75 }, { 9,0.3f,30 }, { 1,1,1 }, 0, -180, 0, 2);
	addCube({ 59,0,92 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -210, 0, 2);
	addCube({ 54,0,99 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -230, 0, 2);
	addCube({ 48,0,103 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -250, 0, 2);
	addCube({ 40,0,105 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -270, 0, 2);
	addCube({ 32,0,104 }, { 9,0.3f,10 }, { 1,1,1 }, 0, -290, 0, 2);
	addCube({ 21,0,94 }, { 9,0.3f,25 }, { 1,1,1 }, 0, -320, 0, 2);
	addCube({ 12,0,84 }, { 9,0.3f,5 }, { 1,1,1 }, 0, -300, 0, 2);
	addCube({ 3.5f,0,83 }, { 9,0.3f,18 }, { 1,1,1 }, 0, -275, 0, 2);
	addCube({ -8,0,84 }, { 9,0.3f,12 }, { 1,1,1 }, 0, -245, 0, 2);
	addCube({ -18,0,92 }, { 8,0.3f,25 }, { 1,1,1 }, 0, -225, 0, 2);
	addCube({ -34,0,110 }, { 8,0.3f,25 }, { 1,1,1 }, 0, -220, 0, 2);
	addCube({ -44,0,120 }, { 8,0.3f,10 }, { 1,1,1 }, 0, -240, 0, 2);
	addCube({ -53,0,122 }, { 8,0.3f,20 }, { 1,1,1 }, 0, -260, 0, 2);
	addCube({ -65,0,122 }, { 8,0.3f,10 }, { 1,1,1 }, 0, -290, 0, 2);
	addCube({ -77,0,100 }, { 7,0.3f,50 }, { 1,1,1 }, 0, -340, 0, 2);


	 //rampas//
	/*addCube({ 0,0,0 }, { 6,0.3f,60 }, { 1,1,1 }, 0, 0, 0);*/
	addCube({ -82,0,50 }, { 4,0.3f,10 }, { 1,1,1 }, 20, 0, 0,2);
	addCube({ -88,0,-15 }, { 4,0.3f,10 }, { 1,1,1 }, 20, 0, 0, 2);
	addCube({ 34,0,-85 }, { 4,0.3f,15 }, { 1,1,1 }, -15, 0, 0, 2);


	for (int i = 0; i < 20; ++i) {

		Cylinder cc = Cylinder(1.0f, 0.2f);
		cc.SetPos(i * 12, 1.5f, i * 5);
		Coin* c = App->physics->AddCoin(cc);
		myCoins.add(c);
		c->Shape->color = Color(1, 1, 0, 1);
		c->collision_listeners.add(this);

	}
	for (int i = 0; i < 3; ++i) {
		
		Cylinder* C = new Cylinder(1, 4);
		C->SetPos(i * 5, 4, i * 6);
		Pipe* P = App->physics->AddPipe(*C);
		P->c1 = C;
		myPipes.add(P);
		Cylinder* CCC = new Cylinder(1, 1);
		P->c2 = CCC;
		P->c2->SetPos(i * 5, 7, i * 6);
		
	}


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	p2List_item<Cube>* c = Borderblocks.getFirst();
	while (c != NULL) {
		c->data.Render();
		c = c->next;
	}


	p2List_item<Coin*>* currentItem = myCoins.getFirst();

	while (currentItem != NULL) {

		if (currentItem->data->pendingToDelete == false) {
			currentItem->data->Update();



			btVector3 coinPos = currentItem->data->GetPosition();
			btVector3 carPos = App->player->vehicle->GetPosition();
			float Xdistance = abs(coinPos.x()) - abs(carPos.x());
			float Ydistance = abs(coinPos.y()) - abs(carPos.y());
			float Zdistance = abs(coinPos.z()) - abs(carPos.z());

			// Homebrew collision detection for sensors
			if ((Xdistance > -2 && Xdistance < 2) && (Ydistance > -2 && Ydistance < 2) && (Zdistance > -2 && Zdistance < 2) && !currentItem->data->pendingToDelete) {
				LOG("car touch coing");
				currentItem->data->pendingToDelete = true;

				currentItem = currentItem->next;
				App->audio->PlayFx(coinFx);
				App->player->extraAcceleration += 50;
				App->player->coins++;

			}
			else {
				currentItem = currentItem->next;
			}
		}
		else {
			currentItem = currentItem->next;
		}
	}

	p2List_item<Pipe*>* currentItemP = myPipes.getFirst();

	while (currentItemP != NULL) {

		if (currentItemP->data->pendingToDelete == false) {
			

			currentItemP->data->c1->Update(currentItemP->data);

			currentItemP = currentItemP->next;
		}
		else {
			currentItemP = currentItemP->next;
		}
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{


	LOG("Collision");
}
