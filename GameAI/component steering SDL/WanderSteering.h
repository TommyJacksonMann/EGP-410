#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~WanderSteering();

	virtual Steering* getSteering();

private:
	float mWanderOffSet;
	float mWanderRadius;
	float mWanderRate;
	float mWanderOrientation;

	FaceSteering* mpFaceSteering;
};