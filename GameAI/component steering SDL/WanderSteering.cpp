#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "MathTools.h"

WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::WANDER;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpFaceSteering = new FaceSteering(mOwnerID, mTargetLoc, mTargetID, false);

	mWanderOffSet = DEFAULT_WANDER_OFFSET;
	mWanderRadius = DEFAULT_WANDER_RADIUS;
	mWanderRate = DEFAULT_WANDER_RATE;
	mWanderOrientation = 0;
}

WanderSteering::~WanderSteering()
{
	delete mpFaceSteering;
}

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mWanderOrientation += genRandomBinomial() * mWanderRate;
	float targetOrientation = mWanderOrientation + pOwner->getFacing() ;
	Vector2D facingVector = getDirectionAsVector(pOwner->getFacing() - .5*PI);
	Vector2D target = pOwner->getPositionComponent()->getPosition() + facingVector*mWanderOffSet;
	target += getDirectionAsVector(targetOrientation) * mWanderRadius;
	mTargetLoc = target;
	mpFaceSteering->setTargetLoc(mTargetLoc);
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	Steering* pSteering = mpFaceSteering->getSteering();
	data.rotAcc = pSteering->getData().rotAcc;
	data.rotVel = pSteering->getData().rotVel;

	diff.normalize();
	data.vel = diff * data.maxSpeed;

	this->mData = data;
	return this;
}



