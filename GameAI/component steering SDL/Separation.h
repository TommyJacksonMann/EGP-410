#pragma once
#include <Trackable.h>
#include "Steering.h"

const float DEFAULT_DECAY_COEFFICIENT = .5;

class Separation : public Steering
{
public:
	Separation(const UnitID& ownerID);

	Vector2D getVelocity();

protected:
};