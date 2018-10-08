#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Cohesion.h"
#include "Separation.h"
#include "Align.h"
#include "WanderSteering.h"

static float DEFAULT_ALIGN_COEFFICIENT = .3;
static float DEFAULT_COHESION_COEFFICIENT = .2;
static float DEFAULT_SEPARATION_COEFFICIENT = .5;

class FlockSteering : public Steering
{
public:
	FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~FlockSteering();

	virtual Steering* getSteering();

	
protected:
	WanderSteering* mpWanderSteering;
	Align* mpAlign;
	Cohesion* mpCohesion;
	Separation* mpSeparation;
};

void alterAlignCoefficient(float value);
void alterCohesionCoefficient(float value);
void alterSeparationCoefficient(float value);