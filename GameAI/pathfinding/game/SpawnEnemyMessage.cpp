#include "SpawnEnemyMessage.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "GameApp.h"
#include "SpriteManager.h"
#include "GridGraph.h"
#include "Grid.h"
#include "GraphicsSystem.h"
#include "./SteeringFiles/PositionComponent.h"

#include "./StateMachingFiles/StateMachine.h"
#include "StateMachingFiles/AiChaseState.h"
#include "StateMachingFiles/AiFleeState.h"
#include "StateMachingFiles/AiWanderState.h"

SpawnEnemyMessage::SpawnEnemyMessage()
	:GameMessage(SPAWN_ENEMY_MESSAGE)
{}

void SpawnEnemyMessage::process()
{
	
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	double currentTime = pGame->getCurrentTime();

	//ALL THIS SPAWNS A COIN WITH A FREQUENCY B
	double enemyDelay = pGame->getEnemySpawnTime();
	int enemyFrequency = pGame->getEnemySpawnFrequency();
	double lastEnemyFired = pGame->getLastEnemySpawned();
	double testEnemyTime = enemyDelay + lastEnemyFired;

	if (testEnemyTime <= currentTime && (rand() % enemyFrequency == 0))
	{
 		int currentEnemiesOnScreen = pGame->getCurrentEnemies();
		int maxEnemiesOnScreen = pGame->getMaxEnemies();
		if (maxEnemiesOnScreen > currentEnemiesOnScreen)
		{
			Grid* pGrid = pGame->getGrid();
			GridGraph* pGridGraph = pGame->getGridGraph();
			//Math for random grid location
			Vector2D newSpawn;

			int randSpawnPoint = rand() % 4;

			switch (randSpawnPoint)
			{
			case 0:
			{
				newSpawn = pGame->getEnemySpawnOne();
				break;
			}
			case 1:
			{
				newSpawn = pGame->getEnemySpawnTwo();
				break;
			}
			case 2:
			{
				newSpawn = pGame->getEnemySpawnThree();
				break;
			}
			case 3:
			{
				newSpawn = pGame->getEnemySpawnFour();
				break;
			}
			}
			Unit* pUnit = gpGame->getUnitManager()->createUnit(*pGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID));
			pUnit->getPositionComponent()->setPosition(newSpawn);
			pUnit->setCollision(CIRCLE, 10);
			pUnit->setSteering(Steering::KINEMATIC_ENEMY_ARRIVE, newSpawn, gpGame->getUnitManager()->getPlayerUnit()->getID());

			pUnit->setUnitType(UnitType::ENEMY);

			StateMachineState* pAiWanderState = new AiWanderState(0, pUnit->getID());
			StateMachineState* pAiChaseState = new AiChaseState(1, pUnit->getID());
			StateMachineState* pAiFleeState = new AiFleeState(2, pUnit->getID());

			pAiWanderState->addTransition(pGame->getChaseTrans());
			pAiWanderState->addTransition(pGame->getFleeTrans());
			pAiChaseState->addTransition(pGame->getWanderTrans());
			pAiChaseState->addTransition(pGame->getFleeTrans());
			pAiFleeState->addTransition(pGame->getChaseTrans());
			pAiFleeState->addTransition(pGame->getWanderTrans());

			pUnit->getStateMachine()->addState(pAiWanderState);
			pUnit->getStateMachine()->addState(pAiChaseState);
			pUnit->getStateMachine()->addState(pAiFleeState);

			pUnit->getStateMachine()->setInitialStateID(0);

			currentEnemiesOnScreen++;

			lastEnemyFired = currentTime;
			pGame->setLastEnemySpawned(lastEnemyFired);
		}

		pGame->setCurrentEnemies(currentEnemiesOnScreen);
	}
}