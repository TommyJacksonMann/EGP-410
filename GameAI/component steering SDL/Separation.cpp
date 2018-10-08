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

Vector2D Separation::getVelocity(vector<Unit*> neighborUnits)
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	Vector2D targetDir(0,0);

	for (int i = 0; i < neighborUnits.size(); i++)
	{
		if (neighborUnits[i] != pOwner)
		{
			Vector2D direction = neighborUnits[i]->getPositionComponent()->getPosition() - pos;
			float distance = direction.getLength();
			if (distance < 30)
			{
				targetDir += direction;
			}
		}
	}
	targetDir *= -1;

	return targetDir;
}

