#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Unit.h"
#include <vector>

using namespace std;

class Cohesion : public Steering
{
public:
	Cohesion(const UnitID& ownerID);

	Vector2D getVelocity(vector<Unit*> neighborUnits);

protected:
};