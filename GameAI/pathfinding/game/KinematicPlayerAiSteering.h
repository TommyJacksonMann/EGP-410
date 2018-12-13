#pragma once

#include "./SteeringFiles/Steering.h"
#include "Path.h"

/*
Class created by Tommy Mann
It is essentially a follow path steering behaviour with AI behaviours in that it decides where it should path find to when it is constructed.
The updatePath() and getSteering() are based off/copied from of RJ's KinematicEnemyChaseSteering functions
/**/
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