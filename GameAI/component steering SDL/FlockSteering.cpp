#include <cassert>

#include "Steering.h"
#include "FlockSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include "MathTools.h"

using namespace std;

FlockSteering::FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	mType = Steering::FLOCK;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpWanderSteering = new WanderSteering(ownerID, targetLoc, targetID);
	mpAlign = new Align(ownerID);
	mpCohesion = new Cohesion(ownerID);
	mpSeparation = new Separation(ownerID);
}

FlockSteering::~FlockSteering()
{
	delete mpWanderSteering;
	delete mpAlign;
	delete mpCohesion;
	delete mpSeparation;
}

void FlockSteering::alterAlignCoefficient(float value)
{
	mAlignCoefficient += value;
}
void FlockSteering::alterCohesionCoefficient(float value)
{
	mCohesionCoefficient += value;
}
void FlockSteering::alterSeparationCoefficient(float value)
{
	mSeparationCoefficient += value;
}

Steering* FlockSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D loc = pOwner->getPositionComponent()->getPosition();
	vector<Unit*> neighborUnits = gpGame->getUnitManager()->getUnitsWithinRadius(loc, DEFAULT_NEIGHBORHOOD_RADIUS, FLOCK);
	bool unitsInNeighborhood = (neighborUnits.size() > 1);

	if (unitsInNeighborhood == false)
	{
		Steering* pSteering = mpWanderSteering->getSteering();
		data = pSteering->getData();
	}
	else
	{
		Vector2D alignVel = mpAlign->getVelocity(neighborUnits);
		alignVel *= mAlignCoefficient;
		Vector2D cohesionVel = mpCohesion->getVelocity(neighborUnits);
		cohesionVel *= mCohesionCoefficient;
		Vector2D separationVel = mpSeparation->getVelocity(neighborUnits);
		separationVel *= mSeparationCoefficient;

		data.vel = alignVel + cohesionVel + separationVel;
		
		data.vel.normalize();
		data.vel *= data.maxSpeed;
		data.acc *= data.maxAccMagnitude;

		float velocityDirection = atan2(data.vel.getY(), data.vel.getX()) + .5f*3.14;
		pOwner->getPositionComponent()->setFacing(velocityDirection);
	}

	this->mData = data;
	return this;
}