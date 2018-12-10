#pragma once

#include "./SteeringFiles/Steering.h"

/*
* Class Implementation by RJ, Similar to the Kinematic Enemy Steer it takes a lot 
* from Tommy's implemnation. The difference is that it gets a heuristic direction 
* and goes in the opposite of that. If theyre is a wall it randomizes which direction it should go but not backwards.
*/

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
