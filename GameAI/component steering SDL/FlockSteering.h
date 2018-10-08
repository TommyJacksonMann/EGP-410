#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Cohesion.h"
#include "Separation.h"
#include "Align.h"
#include "WanderSteering.h"

class FlockSteering : public Steering
{
public:
	FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~FlockSteering();

	virtual Steering* getSteering();

	void alterAlignCoefficient(float value);
	void alterCohesionCoefficient(float value);
	void alterSeparationCoefficient(float value);
protected:
	float mAlignCoefficient = .3;
	float mCohesionCoefficient = .2;
	float mSeparationCoefficient = .5;
	WanderSteering* mpWanderSteering;
	Align* mpAlign;
	Cohesion* mpCohesion;
	Separation* mpSeparation;
};