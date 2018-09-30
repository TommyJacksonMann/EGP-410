#pragma once
#include <Trackable.h>
#include "Steering.h"



class AlignSteering : public Steering
{
public:
	AlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:

private:
	float mTargetRotationRadius;
	float mSlowRotationRadius;
	float mTimeToTargetRotation;
};