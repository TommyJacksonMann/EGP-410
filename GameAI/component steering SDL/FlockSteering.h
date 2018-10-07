#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Cohesion.h"
#include "Separation.h"
#include "Align.h"
#include "WanderSteering.h"

class FlockSteering : public Steering
{
public:
	FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~FlockSteering();

	virtual Steering* getSteering();

protected:
	WanderSteering* mpWanderSteering;
	Align* mpAlign;
	Cohesion* mpCohesion;
	Separation* mpSeparation;
};