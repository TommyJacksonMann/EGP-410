#pragma once
#include <Trackable.h>
#include "Steering.h"

class Align : public Steering
{
public:
	Align(const UnitID& ownerID);

	Vector2D getVelocity();

protected:

};