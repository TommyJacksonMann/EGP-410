#include "DeleteAllCoinsMessage.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "GameApp.h"

DeleteAllCoinsMessage::DeleteAllCoinsMessage()
	: GameMessage(DELETE_COINS_MESSAGE)
{
}

void DeleteAllCoinsMessage::process()
{
	std::map<UnitID, Unit*> tempMap = gpGame->getUnitManager()->getUnitMap();

	auto iter = tempMap.begin();

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	while (iter != tempMap.end())
	{
		if (iter->second->getUnitType() == COIN)
		{
			pGame->getUnitManager()->deleteUnit(iter->second->getID());
			pGame->setCoinsOnScreen(0);
		}
		iter++;
	}
}