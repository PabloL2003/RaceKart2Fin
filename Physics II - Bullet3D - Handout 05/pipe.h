#ifndef __PIPE_H__
#define __PIPE_H__


#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

struct Pipe : public PhysBody3D {

public:
	Primitive* Shape;
	Primitive* Shape2;

	Pipe(btRigidBody* body) :PhysBody3D(body) {};



	void Awake() {

		//this->SetAsSensor(true);

		Shape = new Cylinder(2, 4);
		Shape2 = new Cylinder(2.5f, 1);

		Shape->color = Color(0, 1, 0, 1);
		Shape2->color = Color(0, 1, 0, 1);
	}


	void Update() {


		
		if (body != nullptr){

			
			Shape->Update(this);
			Shape2->Render();

		
		}
	}


};

#endif // __PIPE_H__