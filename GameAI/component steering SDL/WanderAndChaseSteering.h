#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "WanderSteering.h"
#include "SeekSteering.h"

class WanderAndChaseSteering : public Steering
{
public:
	WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~WanderAndChaseSteering();

protected:
	virtual Steering* getSteering();
	

private:
	WanderSteering* mpWanderSteering;
	SeekSteering* mpSeekSteering;

	float mChaseRadius;
};