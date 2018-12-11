#include "HandleCollisionMessage.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/UnitManager.h"
#include "GameMessageManager.h"
#include "RestartGameMessage.h"
#include "PlayerAttackMessage.h"
#include "PlayerRunMessage.h"
#include "./StateMachingFiles/StateMachine.h"
#include "./StateMachingFiles/PlayerAttackState.h"
#include "./StateMachingFiles/PlayerAiAttackState.h"
#include "./StateMachingFiles/PlayerAiRunState.h"
#include <typeinfo>

HandleCollisionMessage::HandleCollisionMessage(Unit* firstUnit, Unit* secondUnit)
	:GameMessage(HANDLE_COLLISION_MESSAGE), 
	mpFirstUnit(firstUnit),
	mpSecondUnit(secondUnit)
{

}

void HandleCollisionMessage::process()
{
	if (mpFirstUnit->getUnitType() == UnitType::PLAYER && mpSecondUnit->getUnitType() == UnitType::COIN)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		pGame->AddScore(ScoreType::COIN_SCORE);
		UnitID coinID = mpSecondUnit->getID();
		pGame->getUnitManager()->deleteUnit(coinID);
		int oldCoinNum = pGame->getCurrentCoinsOnScreen();
		oldCoinNum--;
		pGame->setCoinsOnScreen(oldCoinNum);
	}
	else if (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::COIN)
	{

		GameApp* pGame = static_cast<GameApp*>(gpGame);
		pGame->AddScore(ScoreType::COIN_SCORE);
 		UnitID coinID = mpFirstUnit->getID();
		pGame->getUnitManager()->deleteUnit(coinID);
		int oldCoinNum = pGame->getCurrentCoinsOnScreen();
		oldCoinNum--;
		pGame->setCoinsOnScreen(oldCoinNum);
	}
	else if (mpSecondUnit->getUnitType() == UnitType::COIN && mpFirstUnit->getUnitType() == UnitType::COIN)
	{
		//DELETE COIN ON TOP OF COIN
		//DONT ADD SCORE
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		UnitID coinID = mpFirstUnit->getID();
		pGame->getUnitManager()->deleteUnit(coinID);
		int oldCoinNum = pGame->getCurrentCoinsOnScreen();
		oldCoinNum--;
		pGame->setCoinsOnScreen(oldCoinNum);
		mpSecondUnit->getCollisionComponent()->resetLastCollided();
	}
	else if (mpFirstUnit->getUnitType() == UnitType::PLAYER && mpSecondUnit->getUnitType() == UnitType::ENEMY)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		if (typeid(*mpFirstUnit->getStateMachine()->getCurrentState()) == typeid(PlayerAttackState)  || typeid(*mpFirstUnit->getStateMachine()->getCurrentState()) == typeid(PlayerAiAttackState))
		{
			UnitID enemyID = mpSecondUnit->getID();
			pGame->AddScore(ScoreType::EAT_ENEMY_SCORE);
			pGame->getUnitManager()->deleteUnit(enemyID);
			int oldEnemyNum = pGame->getCurrentEnemies();
			oldEnemyNum--;
			pGame->setCurrentEnemies(oldEnemyNum);
		}
		else
		{
			GameMessage* pMessage = new RestartGameMessage();
			pGame->getMessageManager()->addMessage(pMessage, 0);
			int lives = pGame->getPlayerLives();
			lives--;
			pGame->setPlayerCurrentLives(lives);
			mpSecondUnit->getCollisionComponent()->resetLastCollided();
		}

		
		
	}
	else if (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::ENEMY)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		if (typeid(*mpSecondUnit->getStateMachine()->getCurrentState()) == typeid(PlayerAttackState) || typeid(*mpSecondUnit->getStateMachine()->getCurrentState()) == typeid(PlayerAiAttackState))
		{
			UnitID enemyID = mpFirstUnit->getID();
			pGame->AddScore(ScoreType::EAT_ENEMY_SCORE);
			pGame->getUnitManager()->deleteUnit(enemyID);
			int oldEnemyNum = pGame->getCurrentEnemies();
			oldEnemyNum--;
			pGame->setCurrentEnemies(oldEnemyNum);
		}
		else
		{
			GameMessage* pMessage = new RestartGameMessage();
			pGame->getMessageManager()->addMessage(pMessage, 0);
			int lives = pGame->getPlayerLives();
			lives--;
			pGame->setPlayerCurrentLives(lives);
			mpFirstUnit->getCollisionComponent()->resetLastCollided();
		}
	}
	else if (mpSecondUnit->getUnitType() == UnitType::ENEMY && mpFirstUnit->getUnitType() == UnitType::ENEMY)
	{
		//Nothing
	}
	else if (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::POWER_UP)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		GameMessage* pAttackMessage = new PlayerAttackMessage();
		pGame->getMessageManager()->addMessage(pAttackMessage, 0);
		GameMessage* pRunMessage = new PlayerRunMessage();
		pGame->getMessageManager()->addMessage(pRunMessage, pGame->getPlayerAttackTime());
		mpSecondUnit->getCollisionComponent()->resetLastCollided();
		UnitID powerUpID = mpFirstUnit->getID();
		pGame->getUnitManager()->deleteUnit(powerUpID);
		int oldPowerUpNum = pGame->getCurrentPowerUpsOnScreen();
		oldPowerUpNum--;
		pGame->setPowerUpsOnScreen(oldPowerUpNum);

		pGame->setLastPowerUpFired(pGame->getCurrentTime());
	}
	else if (mpFirstUnit->getUnitType() == UnitType::PLAYER && mpSecondUnit->getUnitType() == UnitType::POWER_UP)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		GameMessage* pAttackMessage = new PlayerAttackMessage();
		pGame->getMessageManager()->addMessage(pAttackMessage, 0);
		GameMessage* pRunMessage = new PlayerRunMessage();
		pGame->getMessageManager()->addMessage(pRunMessage, pGame->getPlayerAttackTime());
		mpFirstUnit->getCollisionComponent()->resetLastCollided();
		UnitID powerUpID = mpSecondUnit->getID();
		pGame->getUnitManager()->deleteUnit(powerUpID);
		int oldPowerUpNum = pGame->getCurrentPowerUpsOnScreen();
		oldPowerUpNum--;
		pGame->setPowerUpsOnScreen(oldPowerUpNum);
		pGame->setLastPowerUpFired(pGame->getCurrentTime());
	}
	else if (mpSecondUnit->getUnitType() == UnitType::POWER_UP && mpFirstUnit->getUnitType() == UnitType::POWER_UP)
	{
		//DELETE POWERUP ON TOP OF POWERUP
		//DONT DO ANYTHING ELSE
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		UnitID powerUpID = mpFirstUnit->getID();
		pGame->getUnitManager()->deleteUnit(powerUpID);
	}

}