#include <cassert>

#include "Steering.h"
#include "Align.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include "MathTools.h"


Align::Align(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::ALIGN;
	setOwnerID(ownerID);

}

Vector2D Align::getVelocity(vector<Unit*> neighborUnits)
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	for (int i = 0; i < neighborUnits.size(); i++)
	{
		if (neighborUnits[i] != pOwner)
		{
			data.vel += neighborUnits[i]->getPhysicsComponent()->getData().vel;
		}
	}

	data.vel /= (neighborUnits.size());

	return data.vel;
}

