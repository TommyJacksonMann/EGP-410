#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>

using namespace std;

FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::FACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::FACE)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	float targetDirection = (atan2(diff.getY(), diff.getX()) + .5f*PI);
	float currentDirection = fmod(pOwner->getFacing(), 2.0*PI);
	if (currentDirection < 0)
	{
		currentDirection = 2 * PI + currentDirection;
	}
	if (targetDirection < 0)
	{
		targetDirection = 2 * PI + targetDirection;
	}

	cout << targetDirection << "\t" << currentDirection << endl;
	
	float rotationSize = targetDirection - currentDirection;
	rotationSize = fmod(rotationSize, 2*PI);
	if (abs(rotationSize) > PI)
	{
		if (currentDirection < targetDirection)
		{
			float temp = -((2 * PI) - rotationSize);
			rotationSize = temp;
		}
		else if (currentDirection > targetDirection)
		{
			float temp = abs((2 * PI) - rotationSize);
			rotationSize = temp;
		}
	}

	float targetRotation;

	if (abs(rotationSize) < mTargetRotationRadius)
	{
		data.rotVel = 0;
		data.rotAcc = 0;
		this->mData = data;
		return this;
	}

	if (abs(rotationSize) > mSlowRotationRadius)
	{
		targetRotation = data.maxRotVel;
	}
	else
	{
		targetRotation = data.maxRotVel * abs(rotationSize) / mSlowRadius;
	}

	data.rotAcc = targetDirection - data.rotVel;
	data.rotAcc /= mTimeToTargetRotation;

	float angularAcceleration = abs(data.rotAcc);
	if (data.rotAcc > data.maxRotAcc)
	{
		data.rotAcc /= abs(data.rotAcc);
		data.rotAcc *= data.maxRotAcc;
	}

	if (rotationSize > 0)
	{
		data.rotAcc = abs(data.rotAcc);
	}
	else if (rotationSize < 0)
	{
		data.rotAcc = -abs(data.rotAcc);
	}

	this->mData = data;
	return this;/**/
}

