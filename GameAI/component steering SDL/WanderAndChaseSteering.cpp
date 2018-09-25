#include <cassert>

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

	mpSeekSteering = new SeekSteering(mOwnerID, mTargetLoc, mTargetID);
	mpWanderSteering = new WanderSteering(mOwnerID, mTargetLoc, mTargetID);

	mChaseRadius = DEFAULT_CHASE_RADIUS;
}

WanderAndChaseSteering::~WanderAndChaseSteering()
{
	delete mpSeekSteering;
	delete mpWanderSteering;
}

Steering* WanderAndChaseSteering::getSteering()
{
	Vector2D diffToPlayer;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D playerLoc;
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		playerLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::WANDERANDCHASE)
	{
		diffToPlayer = playerLoc - pOwner->getPositionComponent()->getPosition();

		if (abs(diffToPlayer.getLength()) <= 20)
		{
			data.rotAcc = 0;
			data.maxRotVel = 0;
			this->mData = data;
			return this;
		}
		
		if (abs(diffToPlayer.getLength()) <= mChaseRadius)
		{
			mTargetLoc = playerLoc;
			mpSeekSteering->setTargetLoc(mTargetLoc);
			Steering* pSteering = mpSeekSteering->getSteering();
			data = pSteering->getData();
		}
		else
		{
			Steering* pSteering = mpWanderSteering->getSteering();
			data = pSteering->getData();
			mTargetLoc = pSteering->getTargetLoc();
		}
		
		this->mData = data;
	}
	return this;
}

