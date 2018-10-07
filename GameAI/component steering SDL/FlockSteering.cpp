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

Steering* FlockSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D loc = pOwner->getPositionComponent()->getPosition();
	bool unitsInNeighborhood = (gpGame->getUnitManager()->getUnitsWithinRadius(loc, 300, FLOCK).size() > 0);

	Steering* pSteering = mpWanderSteering->getSteering();
	data = pSteering->getData();

	if (unitsInNeighborhood == false)
	{
		//Steering* pSteering = mpWanderSteering->getSteering();
		//data = pSteering->getData();
	}
	else
	{
		cout << gpGame->getUnitManager()->getUnitsWithinRadius(loc, 300, FLOCK).size() << endl;
		//Vector2D alignVel = mpAlign->getVelocity();
		//alignVel *= .5;
		//Vector2D cohesionVel = mpCohesion->getVelocity();
		//cohesionVel *= .5;
		//Vector2D separationVel = mpSeparation->getVelocity();
		//separationVel *= 0;

		//data.vel = alignVel + cohesionVel + separationVel;
	}

	this->mData = data;
	return this;
}