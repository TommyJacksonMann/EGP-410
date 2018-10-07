#pragma once
#include <Trackable.h>
#include "Steering.h"

class Cohesion : public Steering
{
public:
	Cohesion(const UnitID& ownerID);

	Vector2D getVelocity();

protected:
};