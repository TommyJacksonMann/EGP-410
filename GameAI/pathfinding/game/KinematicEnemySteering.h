#pragma once
#include "./SteeringFiles/Steering.h"

/*
* Class Implementation by RJ, most of the steering is similar to 
* the kinematic arrive steering that Tommy did. Difference is that it randomizes a
* a direction that isn't the last direction it moved in and has a frequency 
* of when to change direction.
*/

class KinematicEnemySteering :
	public Steering
{
public:
	KinematicEnemySteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);


	virtual Steering* getSteering();

private:
	float mMovementFactor = 0;
	int mLastDirection = -1;
	int mOppositeLastDirection = -1;


	float mChangeDirectionFrequency = 0;
	float mLastChangedDirection = 0;
};

