#pragma once
#include "./SteeringFiles/Steering.h"

class Path;

class FollowPathSteering :
	public Steering
{
public:
	FollowPathSteering(const UnitID& ownerID);
	~FollowPathSteering(); 
	virtual Steering* getSteering();

	void SetPath(Path* pPath) { mpPath = pPath; mCurrentPathPosition = 1; }

protected:

private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
	Path* mpPath;
	int mCurrentPathPosition = 1;
};

