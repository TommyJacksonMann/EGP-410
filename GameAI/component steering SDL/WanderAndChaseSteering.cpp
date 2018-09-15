#include <cassert>

#include "Steering.h"
#include "WanderAndChaseSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderAndChaseSteering::WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::WANDERANDCHASE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* WanderAndChaseSteering::getSteering()
{
	Vector2D diff;
	Vector2D diffToPlayer;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D playerLoc;

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		playerLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::WANDERANDCHASE)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
		diffToPlayer = playerLoc - pOwner->getPositionComponent()->getPosition();

		if (abs(diffToPlayer.getLength()) <= 200)
		{
			mTargetLoc = playerLoc;
		}
		else
		{
			if (abs(diff.getLength()) <= 50)
			{
				mTargetLoc = Vector2D(rand() % 1024, rand() % 768);
			}
		}
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.acc = diff;
	float velocityDirection = atan2(diff.getY(), diff.getX()) + .5f*3.14;
	pOwner->getPositionComponent()->setFacing(velocityDirection);
	this->mData = data;
	return this;
}

