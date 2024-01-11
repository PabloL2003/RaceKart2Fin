#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:

	bool is_sensor = false;
	bool pendingToDelete = false;

	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	btVector3 GetPosition();

	btVector3 GetForwardVector() const
	{
		if (body != nullptr)
		{
			btTransform transform = body->getWorldTransform();
			btMatrix3x3 rotation = transform.getBasis();
			return rotation.getColumn(2);  // Assuming the forward vector is the third column
		}

		// Return a default vector if the body is nullptr
		return btVector3(0.0f, 0.0f, 1.0f);
	}

	void PhysBody3D::SetAsSensor(bool is_sensor)
	{
		if (this->is_sensor != is_sensor)
		{
			this->is_sensor = is_sensor;
			if (is_sensor == true)
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			else
				body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}

public:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
	int id = 0;
};

#endif // __PhysBody3D_H__