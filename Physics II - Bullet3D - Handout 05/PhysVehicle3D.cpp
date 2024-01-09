#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
    // Render kart body
    Cube mainBody(info.chassis_size.x, info.chassis_size.y/4, info.chassis_size.z);
    vehicle->getChassisWorldTransform().getOpenGLMatrix(&mainBody.transform);
    btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
    btVector3 offset(info.chassis_offset.x, info.chassis_offset.y/4, info.chassis_offset.z);
    offset = offset.rotate(q.getAxis(), q.getAngle());

    mainBody.transform.M[12] += offset.getX();
    mainBody.transform.M[13] += offset.getY();
    mainBody.transform.M[14] += offset.getZ();

    mainBody.color = Red; // Set the color of the body
    mainBody.Render();

    // Render additional cubes to form the kart body
    Cube frontSpoiler(info.chassis_size.x * 0.8f, info.chassis_size.y * 0.1f, info.chassis_size.z * 0.2f);
    frontSpoiler.color = Red;
    frontSpoiler.SetPos(mainBody.transform.M[12], mainBody.transform.M[13] + info.chassis_size.y * 0.5f, mainBody.transform.M[14] + info.chassis_size.z * 0.5f);
    frontSpoiler.Render();

    Cube rearSpoiler(info.chassis_size.x * 0.8f, info.chassis_size.y * 0.1f, info.chassis_size.z * 0.2f);
    rearSpoiler.color = Red;
    rearSpoiler.SetPos(mainBody.transform.M[12], mainBody.transform.M[13] + info.chassis_size.y * 0.5f, mainBody.transform.M[14] - info.chassis_size.z * 0.5f);
    rearSpoiler.Render();

    // Render kart wheels
    Cylinder wheel;
    wheel.color = Blue;

    for (int i = 0; i < vehicle->getNumWheels(); ++i)
    {
        wheel.radius = info.wheels[0].radius;
        wheel.height = info.wheels[0].width;

        vehicle->updateWheelTransform(i);
        vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

        wheel.Render();
    }
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}