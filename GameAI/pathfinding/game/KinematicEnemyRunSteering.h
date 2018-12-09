#pragma once

#include "./SteeringFiles/Steering.h"

class KinematicEnemyRunSteering : public Steering
{
public :
	KinematicEnemyRunSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~KinematicEnemyRunSteering() {}

	virtual Steering* getSteering();
private:
	float mMovementFactor = 0;
	int mLastDirection = -1;

	Vector2D mTargetPlayerPos;
};
