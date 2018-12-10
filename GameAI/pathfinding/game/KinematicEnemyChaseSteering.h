#pragma once


#include "./SteeringFiles/Steering.h"
#include "Path.h"


class KinematicEnemyChaseSteering : public Steering
{
public:
	KinematicEnemyChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~KinematicEnemyChaseSteering() {}

	virtual Steering* getSteering();


private:
	
	void updatePath();

	float mMovementFactor = 0;
	
	Path mPath;
	int mCurrentPathPosition = 1;

	Vector2D mTargetPlayerPos;

	bool mJustChangedPath = false;


};