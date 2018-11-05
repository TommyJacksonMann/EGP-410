#pragma once
#include "./SteeringFiles/Steering.h"
#include "Path.h"

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
	Path mpPath;
	int mCurrentPathPosition = 1;
};

