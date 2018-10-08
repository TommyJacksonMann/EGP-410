#include "Game.h"
#include "GameMessageManager.h"
#include "AlterFlockMessage.h"
#include <vector>
#include "UnitManager.h"
#include "SteeringComponent.h"
#include "FlockSteering.h"

AlterFlockMessage::AlterFlockMessage(FlockData data, float value)
	:GameMessage(ALTER_FLOCK_MESSAGE)
	,mDataToAlter(data)
	,mValue(value)
{
}

AlterFlockMessage::~AlterFlockMessage()
{
}

void AlterFlockMessage::process()
{

	switch (mDataToAlter)
	{
	case FlockData::ALIGN:
		alterAlignCoefficient(mValue);
		break;
	case FlockData::COHESION:
		alterCohesionCoefficient(mValue);
		break;
	case FlockData::SEPARATION:
		alterSeparationCoefficient(mValue);
		break;
	}

}