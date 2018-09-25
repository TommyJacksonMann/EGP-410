#include <cassert>

#include "Steering.h"
#include "ArriveAndFaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVEANDFACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpArriveSteering = new ArriveSteering(mOwnerID, mTargetLoc, mTargetID);
	mpFaceSteering = new FaceSteering(mOwnerID, mTargetLoc, mTargetID);

	mTargetRadius = DEFAULT_TARGET_RADIUS;
	mSlowRadius = DEFAULT_SLOW_RADIUS;
	mTimeToTarget = DEFAULT_TIME_TO_TARGET;
}

ArriveAndFaceSteering::~ArriveAndFaceSteering()
{
	delete mpArriveSteering;
	delete mpFaceSteering;
}

Steering* ArriveAndFaceSteering::getSteering()
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

	if (mType == Steering::ARRIVEANDFACE)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	
	Steering* pSteering = mpFaceSteering->getSteering();
	data.rotAcc = pSteering->getData().rotAcc;
	data.rotVel = pSteering->getData().rotVel;

	float targetSpeed;
	Vector2D targetVelocity;
	Vector2D facingVector = getDirectionAsVector(pOwner->getFacing() - .5*PI);

	if (diff.getLength() < mTargetRadius)
	{
		data.vel = Vector2D(0, 0);
		data.acc = Vector2D(0, 0);
		this->mData = data;
		return this;
	}

	if (diff.getLength() > mSlowRadius)
	{
		targetSpeed = data.maxSpeed;
	}
	else
	{
		targetSpeed = data.maxSpeed * diff.getLength() / mSlowRadius;
	}

	targetVelocity = facingVector;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	data.acc = targetVelocity - data.vel;
	data.acc /= mTimeToTarget;

	if (data.acc.getLength() > data.maxAccMagnitude)
	{
		data.acc.normalize();

		data.acc *= data.maxAccMagnitude;

	}

	this->mData = data;
	return this;
}

Vector2D ArriveAndFaceSteering::getDirectionAsVector(float direction)
{
	float x = cos(direction);
	float y = sin(direction);

	return Vector2D(x, y);
}