#include <cassert>

#include "Steering.h"
#include "Separation.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include "MathTools.h"

using namespace std;

Separation::Separation(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::SEPARATION;
	setOwnerID(ownerID);

}

Vector2D Separation::getVelocity()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D pos = pOwner->getPositionComponent()->getPosition();

	std::vector<Unit*> neighborUnits = gpGame->getUnitManager()->getUnitsWithinRadius(pos, 300, FLOCK);

	for (int i = 0; i < neighborUnits.size(); i++)
	{
		if (neighborUnits[i] != pOwner)
		{
			Vector2D direction = neighborUnits[i]->getPositionComponent()->getPosition() - pos;
			float distance = direction.getLength();
			float strength = min(DEFAULT_DECAY_COEFFICIENT/(distance * distance), data.maxAccMagnitude);
			direction.normalize();
			direction *= strength;
			data.acc += direction;
		}
	}

	return data.vel;
}

