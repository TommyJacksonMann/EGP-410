#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "FaceSteering.h"

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
}

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mWanderOrientation += genRandomBinomial() * mWanderRate;
	float targetOrientation = mWanderOrientation + pOwner->getFacing();
	Vector2D facingVector = getDirectionVector(pOwner->getFacing());
	Vector2D target = pOwner->getPositionComponent()->getPosition() + facingVector*mWanderOffSet;
	target += getDirectionVector(mWanderOrientation) * mWanderRadius;
	FaceSteering* faceData = new FaceSteering();

	float velocityDirection = atan2(diff.getY(), diff.getX()) + .5f*3.14;
	pOwner->getPositionComponent()->setFacing(velocityDirection);
	this->mData = data;
	return this;
}

Vector2D WanderSteering::getDirectionVector(float direction)
{
	float x = cos(direction);
	float y = sin(direction);

	return Vector2D(x, y);
}

