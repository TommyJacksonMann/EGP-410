#pragma once
#include "./SteeringFiles/Steering.h"
#include "Path.h"

class FlowFieldSteering : public Steering
{
public:
	FlowFieldSteering(const UnitID& ownerID);
	~FlowFieldSteering();
	virtual Steering* getSteering();

	void SetNode(Node* pNode);
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
	Node* mpCurrentNode;
};

