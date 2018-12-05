#pragma once
#include <Trackable.h>
#include "./SteeringFiles/Steering.h"

const float DEFAULT_MOVEMENT_FACTOR = 5;

class KinematicArriveSteering : public Steering
{
public:
	KinematicArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:

private:
	float mMovementFactor = 0;
};