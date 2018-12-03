#include "HandleCollisionMessage.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"

HandleCollisionMessage::HandleCollisionMessage(Unit* firstUnit, Unit* secondUnit)
	:GameMessage(HANDLE_COLLISION_MESSAGE), 
	mpFirstUnit(firstUnit),
	mpSecondUnit(secondUnit)
{

}

void HandleCollisionMessage::process()
{
	if ((mpFirstUnit->getUnitType() == UnitType::PLAYER && mpSecondUnit->getUnitType() == UnitType::COIN) 
		|| (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::COIN))
	{
		std::cout << "COLLIDING";
	}
}