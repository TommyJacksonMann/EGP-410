#include "Game.h"
#include "GameMessageManager.h"
#include "AlterFlockMessage.h"

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

}