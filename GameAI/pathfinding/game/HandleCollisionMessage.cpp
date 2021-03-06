#include "HandleCollisionMessage.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/UnitManager.h"
#include "ResetPlayerStartPosMessage.h"
#include "GameMessageManager.h"

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
		UnitID coinID = mpSecondUnit->getID();
		pGame->getUnitManager()->deleteUnit(coinID);
		int oldCoinNum = pGame->getCurrentCoinsOnScreen();
		oldCoinNum--;
		pGame->setCoinsOnScreen(oldCoinNum);
	}
	else if (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::COIN)
	{

		GameApp* pGame = static_cast<GameApp*>(gpGame);
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
	}
	else if (mpFirstUnit->getUnitType() == UnitType::PLAYER && mpSecondUnit->getUnitType() == UnitType::ENEMY)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		GameMessage* pMessage = new ResetPlayerStartPos();
		pGame->getMessageManager()->addMessage(pMessage, 0);

	}
	else if (mpSecondUnit->getUnitType() == UnitType::PLAYER && mpFirstUnit->getUnitType() == UnitType::ENEMY)
	{
		GameApp* pGame = static_cast<GameApp*>(gpGame);
		GameMessage* pMessage = new ResetPlayerStartPos();
		pGame->getMessageManager()->addMessage(pMessage, 0);
	}
	else if (mpSecondUnit->getUnitType() == UnitType::ENEMY && mpFirstUnit->getUnitType() == UnitType::ENEMY)
	{
		//Nothing
	}

}