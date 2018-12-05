#include <cassert>

#include "./SteeringFiles/Steering.h"
#include "KinematicArriveSteering.h"
#include "Game.h"
#include "./SteeringFiles/UnitManager.h"
#include "./SteeringFiles/Unit.h"
#include <iostream>

using namespace std;

KinematicArriveSteering::KinematicArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::KINEMATICARRIVE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mMovementFactor = 6;
}

Steering* KinematicArriveSteering::getSteering()
{
	Vector2D targetDirection;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	targetDirection = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float targetSpeed;
	Vector2D targetVelocity;

	if (targetDirection.getLength() < mMovementFactor)
	{
		pOwner->getPositionComponent()->setPosition(mTargetLoc);
	}
	else
	{
		targetDirection.normalize();

		targetVelocity = targetDirection;
		targetVelocity *= mMovementFactor;
		pOwner->getPositionComponent()->setPosition(pOwner->getPositionComponent()->getPosition() + targetVelocity);
		float velocityDirection = atan2(targetDirection.getY(), targetDirection.getX()) + .5f*3.14;
		pOwner->getPositionComponent()->setFacing(velocityDirection);
	}
	
	//data.vel = targetVelocity;

	

	this->mData = data;
	return this;
}

