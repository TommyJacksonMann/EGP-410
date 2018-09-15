#include <cassert>

#include "Steering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveSteering::getSteering()
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

	if (mType == Steering::ARRIVE)
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

	if (unnormalizedDiff.getLength() < 20)
	{
		data.acc = 0;
		data.vel = 0;
	}
	else
	{
		data.acc = diff;
	}
	
	float velocityDirection = atan2(diff.getY(), diff.getX()) + .5f*3.14;
	pOwner->getPositionComponent()->setFacing(velocityDirection);
	this->mData = data;
	return this;
}

