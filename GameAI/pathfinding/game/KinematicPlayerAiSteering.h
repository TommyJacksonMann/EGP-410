#pragma once

#include "./SteeringFiles/Steering.h"
#include "Path.h"


class KinematicPlayerAiSteering : public Steering
{
public:
	KinematicPlayerAiSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~KinematicPlayerAiSteering() {}

	virtual Steering* getSteering();

	void determineDestination();
private:

	void updatePath();

	float mMovementFactor = 0;

	Path mPath;
	int mCurrentPathPosition = 1;

	Vector2D mTargetPlayerPos;

	bool mJustChangedPath = false;


};