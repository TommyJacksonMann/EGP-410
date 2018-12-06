#include "ResetPlayerStartPosMessage.h"
#include "GameApp.h"
#include "./SteeringFiles/UnitManager.h"
#include "DataParser.h"
#include "./SteeringFiles/Steering.h"

ResetPlayerStartPos::ResetPlayerStartPos()
	: GameMessage(RESET_PLAYER_POS_MESSAGE)
{
}

void ResetPlayerStartPos::process()
{
	Vector2D playerSpawn(DataParser::getInstance()->ReadFile("PlayerSpawnX"), DataParser::getInstance()->ReadFile("PlayerSpawnY"));
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();
	pPlayer->getPositionComponent()->setPosition(playerSpawn);
	pPlayer->setSteering(Steering::KINEMATICARRIVE, playerSpawn);
	pPlayer->getCollisionComponent()->resetLastCollided();
}
