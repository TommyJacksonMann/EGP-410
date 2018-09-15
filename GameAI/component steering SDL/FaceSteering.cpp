#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>

const float PI = 3.14156;
const float RAD2DEG = 180 / PI;
const float DEG2RAD = PI / 180;

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

	Vector2D unnormalizedDiff = diff;

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	float targetDirection = (atan2(diff.getY(), diff.getX()) + .5f*PI) ;
	float currentDirection = fmod(pOwner->getFacing(), 2.0*PI);

	if (abs(targetDirection - currentDirection) < 3*DEG2RAD)
	{
		data.rotVel = 0;
		data.rotAcc = 0;
	}
	else if(targetDirection - currentDirection < 0)
	{
		data.rotAcc = -1;
	}
	else if (targetDirection - currentDirection > 0)
	{
		data.rotAcc = 1;
	}

	this->mData = data;
	return this;
}

