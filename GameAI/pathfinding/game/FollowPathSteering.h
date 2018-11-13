#pragma once
#include "./SteeringFiles/Steering.h"
#include "Path.h"

//Steering class for the unit to go to each node in a path. Created by RJ Bourdelais

class FollowPathSteering :	public Steering
{
public:
	FollowPathSteering(const UnitID& ownerID);
	~FollowPathSteering(); 
	virtual Steering* getSteering();

	void SetPath(Path* pPath);
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
	Path mPath;
	int mCurrentPathPosition = 1;
};

