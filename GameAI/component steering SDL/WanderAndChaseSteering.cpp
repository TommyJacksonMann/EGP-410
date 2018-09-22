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

	mpArriveSteering = new ArriveSteering(mOwnerID, mTargetLoc, mTargetID);
	mpWanderSteering = new WanderSteering(mOwnerID, mTargetLoc, mTargetID);
}

WanderAndChaseSteering::~WanderAndChaseSteering()
{
	delete mpArriveSteering;
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

		if (abs(diffToPlayer.getLength()) <= 300)
		{
			mTargetLoc = playerLoc;
			mpArriveSteering->setTargetLoc(mTargetLoc);
			Steering* pSteering = mpArriveSteering->getSteering();
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

