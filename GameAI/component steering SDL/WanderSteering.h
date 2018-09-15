#pragma once
#include <Trackable.h>
#include "Steering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();
};