#pragma once

#include "GameMessage.h"
#include "./SteeringFiles/Unit.h"

/* Class implementation by RJ
* Compares the two units and handles collision depending on the types of units
*/

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