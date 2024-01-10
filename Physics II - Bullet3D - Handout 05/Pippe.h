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

struct Pipe {

public:
	Primitive* Shape;
	Primitive* Shape2;
	PhysBody3D* body;
	PhysBody3D* body2;

	Pipe(PhysBody3D* b, PhysBody3D* b2, Color aColor, int height) {
		body = b;
		body2 = b2;
		Shape = new Cylinder(2, height);
		Shape2 = new Cylinder(2.5f, 1);



		Shape->color =  aColor;
		Shape2->color = aColor;
	}




	void Update() {



		if (body != nullptr) {

			btVector3 zeroVelocity(0, 0, 0);
			body->body->setLinearVelocity(zeroVelocity);
			body2->body->setLinearVelocity(zeroVelocity);
			Shape->Update(body);
			Shape2->Update(body2);


		}
	}


};

#endif // __PIPE_H__