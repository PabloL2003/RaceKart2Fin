#ifndef __COIN_H__
#define __COIN_H__


#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

struct Coin : public PhysBody3D {

public:
	Primitive* Shape;

	Coin(btRigidBody* body) :PhysBody3D(body) {};
	

	
	void Awake() {

		this->SetAsSensor(true);
		Shape = new Cylinder(1, 0.2f);
	}


	void Update() {

		if (body != nullptr && !pendingToDelete) {

			Shape->Update(this);

			btVector3 speed = body->getAngularVelocity();
			speed.setY(10);

			body->setAngularVelocity(speed);

			btVector3 zeroVelocity(0, 0, 0);
			body->setLinearVelocity(zeroVelocity);
			body->setGravity(zeroVelocity);
			//btTransform trans;
			//body->getMotionState()->getWorldTransform(trans);

			//// Incremental rotation around the Y-axis
			//btQuaternion rotationIncrement(btVector3(0, 1, 0), 0.01);
			//trans.setRotation(rotationIncrement );

			//body->getMotionState()->setWorldTransform(trans);
		}
	}


};



#endif // __COIN_H__