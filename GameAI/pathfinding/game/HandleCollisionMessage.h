#pragma once

#include "GameMessage.h"
#include "./SteeringFiles/Unit.h"

class HandleCollisionMessage
	: public GameMessage
{
public:
	HandleCollisionMessage(Unit*, Unit*);
	~HandleCollisionMessage() {}

	void process();
private:
	Unit* mpFirstUnit;
	Unit* mpSecondUnit;

};