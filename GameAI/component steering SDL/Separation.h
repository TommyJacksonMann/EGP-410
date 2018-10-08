#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Unit.h"
#include <vector>

using namespace std;

const float DEFAULT_DECAY_COEFFICIENT = 100000;

class Separation : public Steering
{
public:
	Separation(const UnitID& ownerID);

	Vector2D getVelocity(vector<Unit*> neighborUnits);

protected:
};