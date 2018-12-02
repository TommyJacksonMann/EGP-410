#pragma once
#include <Trackable.h>
#include "./SteeringFiles/Steering.h"

const float KINEMATIC_NUM_FRAMES_TO_TARGET = 3;

class KinematicArriveSteering : public Steering
{
public:
	KinematicArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:

private:
	float mMovementFactor = 0;
};