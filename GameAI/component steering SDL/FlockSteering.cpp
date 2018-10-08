#include <cassert>

#include "Steering.h"
#include "FlockSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include "MathTools.h"
#include "FileSystem.h"

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

	DEFAULT_ALIGN_COEFFICIENT = FileSystem::getInstance()->loadInfoFloat("AlignWeight");
	DEFAULT_COHESION_COEFFICIENT = FileSystem::getInstance()->loadInfoFloat("CohesionWeight");
	DEFAULT_SEPARATION_COEFFICIENT = FileSystem::getInstance()->loadInfoFloat("SeparationWeight");
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
	vector<Unit*> neighborUnits = gpGame->getUnitManager()->getUnitsWithinRadius(loc, DEFAULT_NEIGHBORHOOD_RADIUS, FLOCK);
	bool unitsInNeighborhood = (neighborUnits.size() > 1);

	cout << DEFAULT_ALIGN_COEFFICIENT << "\t" << DEFAULT_COHESION_COEFFICIENT << "\t" << DEFAULT_SEPARATION_COEFFICIENT << endl;
	if (unitsInNeighborhood == false)
	{
		Steering* pSteering = mpWanderSteering->getSteering();
		data = pSteering->getData();
	}
	else
	{
		Vector2D alignVel = mpAlign->getVelocity(neighborUnits);
		alignVel *= DEFAULT_ALIGN_COEFFICIENT;
		Vector2D cohesionVel = mpCohesion->getVelocity(neighborUnits);
		cohesionVel *= DEFAULT_COHESION_COEFFICIENT;
		Vector2D separationVel = mpSeparation->getVelocity(neighborUnits);
		separationVel *= DEFAULT_SEPARATION_COEFFICIENT;

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

void alterAlignCoefficient(float value)
{
	DEFAULT_ALIGN_COEFFICIENT += value;
	if (DEFAULT_ALIGN_COEFFICIENT < 0)
	{
		DEFAULT_ALIGN_COEFFICIENT = 0;
	}
	if (DEFAULT_ALIGN_COEFFICIENT > 1)
	{
		DEFAULT_ALIGN_COEFFICIENT = 1;
	}
	FileSystem::getInstance()->overWriteFloat("AlignWeight", DEFAULT_ALIGN_COEFFICIENT);

}
void alterCohesionCoefficient(float value)
{
	DEFAULT_COHESION_COEFFICIENT += value;
	if (DEFAULT_COHESION_COEFFICIENT < 0)
	{
		DEFAULT_COHESION_COEFFICIENT = 0;
	}
	if (DEFAULT_COHESION_COEFFICIENT > 1)
	{
		DEFAULT_COHESION_COEFFICIENT = 1;
	}
	FileSystem::getInstance()->overWriteFloat("CohesionWeight", DEFAULT_COHESION_COEFFICIENT);
}
void alterSeparationCoefficient(float value)
{
	DEFAULT_SEPARATION_COEFFICIENT += value;
	if (DEFAULT_SEPARATION_COEFFICIENT < 0)
	{
		DEFAULT_SEPARATION_COEFFICIENT = 0;
	}
	if (DEFAULT_SEPARATION_COEFFICIENT > 1)
	{
		DEFAULT_SEPARATION_COEFFICIENT = 1;
	}
	FileSystem::getInstance()->overWriteFloat("SeparationWeight", DEFAULT_SEPARATION_COEFFICIENT);
}