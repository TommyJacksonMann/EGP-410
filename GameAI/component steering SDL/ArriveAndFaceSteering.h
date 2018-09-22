#pragma once
#include "FaceSteering.h"
#include "ArriveSteering.h"

class ArriveAndFaceSteering : public Steering
{
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~ArriveAndFaceSteering();

protected:
	virtual Steering* getSteering();

private:
	FaceSteering* mpFaceSteering;
	ArriveSteering* mpArriveSteering;
};