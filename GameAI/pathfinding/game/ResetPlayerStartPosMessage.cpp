#include "ResetPlayerStartPosMessage.h"
#include "GameApp.h"
#include "./SteeringFiles/UnitManager.h"
#include "DataParser.h"
#include "./SteeringFiles/Steering.h"
#include "./SteeringFiles/SteeringComponent.h"
#include "StateMachingFiles/StateMachine.h"
#include "StateMachingFiles/PlayerAiAttackState.h"
#include "KinematicArriveSteering.h"
#include "KinematicPlayerAiSteering.h"

ResetPlayerStartPos::ResetPlayerStartPos()
	: GameMessage(RESET_PLAYER_POS_MESSAGE)
{
}

void ResetPlayerStartPos::process()
{
	Vector2D playerSpawn(DataParser::getInstance()->ReadFile("PlayerSpawnX"), DataParser::getInstance()->ReadFile("PlayerSpawnY"));
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();
	pPlayer->getPositionComponent()->setPosition(playerSpawn);
	Steering* pPlayerSteering = pPlayer->getSteeringComponent()->getSteering();
	if (typeid(*pPlayerSteering) == typeid(KinematicArriveSteering))
	{
		pPlayer->setSteering(Steering::KINEMATICARRIVE, playerSpawn);
	}
	else if (typeid(*pPlayerSteering) == typeid(KinematicPlayerAiSteering))
	{
		pPlayer->setSteering(Steering::KINEMATIC_PLAYER_AI, playerSpawn, pPlayer->getID());
	}
	pPlayer->getCollisionComponent()->resetLastCollided();
}
