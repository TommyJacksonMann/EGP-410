#pragma once
#include <Trackable.h>
#include "Steering.h"

class WanderAndChaseSteering : public Steering
{
public:
	WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();
	bool mChasing = false;
};