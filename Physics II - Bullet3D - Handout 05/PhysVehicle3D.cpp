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
    Cube mainBody(info.chassis_size.x, info.chassis_size.y/8, info.chassis_size.z);
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

    frontSpoiler.color = White;
    btTransform frontSpoilerTransform = vehicle->getChassisWorldTransform();
    btQuaternion frontSpoilerRotation = frontSpoilerTransform.getRotation();
    btVector3 frontSpoilerOffset(0.0f, info.chassis_size.y * 0.3f, info.chassis_size.z * 0.5f);
    frontSpoilerOffset = frontSpoilerOffset.rotate(frontSpoilerRotation.getAxis(), frontSpoilerRotation.getAngle());
    frontSpoilerTransform.setOrigin(frontSpoilerTransform.getOrigin() + frontSpoilerOffset);
    frontSpoilerTransform.setRotation(frontSpoilerRotation);
    frontSpoilerTransform.getOpenGLMatrix(&frontSpoiler.transform);
    frontSpoiler.Render();

    // Rotation angle for the aileron effect
    float aileronAngle = 0.4f;

    // Rotation angle for the front bumper
    float bumperAngle = 0.9f;

    // Render rear spoiler as an aileron
    Cube rearSpoiler(info.chassis_size.x * 0.7f, info.chassis_size.y * 0.1f, info.chassis_size.z * 0.2f);
    rearSpoiler.color = White;
    btTransform rearSpoilerTransform = vehicle->getChassisWorldTransform();
    btQuaternion rearSpoilerRotation = rearSpoilerTransform.getRotation();
    btVector3 rearSpoilerOffset(0.0f, info.chassis_size.y * 0.5f, -info.chassis_size.z * 0.5f);
    rearSpoilerOffset = rearSpoilerOffset.rotate(rearSpoilerRotation.getAxis(), rearSpoilerRotation.getAngle());
    rearSpoilerTransform.setOrigin(rearSpoilerTransform.getOrigin() + rearSpoilerOffset);

    // Apply aileron effect to the rear spoiler
    rearSpoilerRotation *= btQuaternion(btVector3(1, 0, 0), aileronAngle);
    rearSpoilerTransform.setRotation(rearSpoilerRotation);

    rearSpoilerTransform.getOpenGLMatrix(&rearSpoiler.transform);
    rearSpoiler.Render();

    //Render aileron union
    Cube aileronUnion(info.chassis_size.x * 0.1f, info.chassis_size.y * 0.1f, info.chassis_size.z * 0.1f);
    aileronUnion.color = White;
    btTransform aileronUnionTransform = vehicle->getChassisWorldTransform();
    btQuaternion aileronUnionRotation = aileronUnionTransform.getRotation();
    btVector3 aileronUnionOffset(0.0f, info.chassis_size.y * 0.4f, -info.chassis_size.z * 0.45f);
    aileronUnionOffset = aileronUnionOffset.rotate(aileronUnionRotation.getAxis(), frontSpoilerRotation.getAngle());
    aileronUnionTransform.setOrigin(aileronUnionTransform.getOrigin() + aileronUnionOffset);
    aileronUnionTransform.setRotation(aileronUnionRotation);
    aileronUnionTransform.getOpenGLMatrix(&aileronUnion.transform);
    aileronUnion.Render();

    // Render front bumper
    Cube frontBumper(info.chassis_size.x * 0.9f, info.chassis_size.y * 0.1f, info.chassis_size.z * 0.2f);
    frontBumper.color = White;
    btTransform frontBumperTransform = vehicle->getChassisWorldTransform();
    btQuaternion frontBumperRotation = frontBumperTransform.getRotation();
    btVector3 frontBumperOffset(0.0f, info.chassis_size.y * 0.4f, info.chassis_size.z * 0.5f);
    frontBumperOffset = frontBumperOffset.rotate(frontBumperRotation.getAxis(), frontBumperRotation.getAngle());
    frontBumperTransform.setOrigin(frontBumperTransform.getOrigin() + frontBumperOffset);

    // Apply aileron effect to the rear spoiler
    frontBumperRotation *= btQuaternion(btVector3(1, 0, 0), bumperAngle);
    frontBumperTransform.setRotation(frontBumperRotation);

    frontBumperTransform.getOpenGLMatrix(&frontBumper.transform);
    frontBumper.Render();
    
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