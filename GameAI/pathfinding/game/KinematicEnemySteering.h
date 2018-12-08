#pragma once
#include "./SteeringFiles/Steering.h"
class KinematicEnemySteering :
	public Steering
{
public:
	KinematicEnemySteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);


	virtual Steering* getSteering();

private:
	float mMovementFactor = 0;
};

