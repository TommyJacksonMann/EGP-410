#pragma once
#include "./SteeringFiles/Steering.h"
#include "Path.h"


class KinematicFollowPathSteering : public Steering
{
public:
	KinematicFollowPathSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~KinematicFollowPathSteering() {}

	virtual Steering* getSteering();


private:

	void updatePath();

	float mMovementFactor = 0;

	Path mPath;
	int mCurrentPathPosition = 1;

	Vector2D mTargetPlayerPos;

	bool mJustChangedPath = false;


};