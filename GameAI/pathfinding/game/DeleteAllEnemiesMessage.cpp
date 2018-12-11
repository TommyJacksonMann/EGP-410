#include "DeleteAllEnemiesMessage.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "GameApp.h"


DeleteAllEnemiesMessage::DeleteAllEnemiesMessage()
	:GameMessage(DELETE_ENEMY_MESSAGE)
{
}


DeleteAllEnemiesMessage::~DeleteAllEnemiesMessage()
{
}
void DeleteAllEnemiesMessage::process()
{
	std::map<UnitID, Unit*> tempMap = gpGame->getUnitManager()->getUnitMap();

	auto iter = tempMap.begin();

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	while (iter != tempMap.end())
	{
		if (iter->second->getUnitType() == ENEMY)
		{
			pGame->getUnitManager()->deleteUnit(iter->second->getID());
			
		}
		iter++;
	}
	pGame->setCurrentEnemies(0);
}
