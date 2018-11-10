#include <cassert>

#include "Steering.h"
#include "Cohesion.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include "MathTools.h"

using namespace std;

Cohesion::Cohesion(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::COHESION;
	setOwnerID(ownerID);

}

Vector2D Cohesion::getVelocity(vector<Unit*> neighborUnits)
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D pos = pOwner->getPositionComponent()->getPosition();


	Vector2D medianPoint = Vector2D(0,0);
	if (neighborUnits.size() <= 2)
	{
		return Vector2D(0,0);
	}

	for (int i = 0; i < neighborUnits.size(); i++)
	{
		if (neighborUnits[i] != pOwner)
		{
			medianPoint += neighborUnits[i]->getPositionComponent()->getPosition();
		}
	}
	medianPoint /= (neighborUnits.size() - 1);

	data.vel = Vector2D(medianPoint.getX() - pos.getX(), medianPoint.getY() - pos.getY());
	data.vel.normalize();

	return data.vel;
}

