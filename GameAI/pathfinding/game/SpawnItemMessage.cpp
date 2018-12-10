#include "SpawnItemMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles//UnitManager.h"
#include "SpriteManager.h"
#include "GridGraph.h"
#include "Grid.h"
#include "GraphicsSystem.h"
#include "./SteeringFiles/PositionComponent.h"


SpawnItemMessage::SpawnItemMessage()
	: GameMessage(SPAWN_ITEM_MESSAGE)
{

}

SpawnItemMessage::~SpawnItemMessage()
{

}

void SpawnItemMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	double currentTime = pGame->getCurrentTime();

	//ALL THIS SPAWNS A COIN WITH A FREQUENCY B
	double coinDelay = pGame->getCoinDelay();
	int coinFrequency = pGame->getCoinFrequency();
	double lastCoinFired = pGame->getLastCoinFired();
	double testCoinTime = coinDelay + lastCoinFired;

	if (testCoinTime <= currentTime && (rand() % coinFrequency == 0))
	{
		int currentCoinsOnScreen = pGame->getCurrentCoinsOnScreen();
		int maxCoinsOnScreen = pGame->getMaxCoinsOnScreen();
		if (maxCoinsOnScreen > currentCoinsOnScreen)
		{
			Grid* pGrid = pGame->getGrid();
			GridGraph* pGridGraph = pGame->getGridGraph();
			//Math for random grid location
			int posX = (rand() * 32) % pGame->getGraphicsSystem()->getWidth();
			int posY = (rand() * 32) % pGame->getGraphicsSystem()->getHeight();
			while (pGrid->getValueAtIndex(pGrid->getSquareIndexFromPixelXY(posX, posY)) == BLOCKING_VALUE)
			{
				posX = (rand() * 32) % gpGame->getGraphicsSystem()->getWidth();
				posY = (rand() * 32) % gpGame->getGraphicsSystem()->getHeight();
			}
			Unit* pUnit = gpGame->getUnitManager()->createUnit(*pGame->getSpriteManager()->getSprite(COIN_SPRITE_ID));
			pUnit->getPositionComponent()->setPosition(Vector2D(posX, posY));
			pUnit->setCollision(CIRCLE, 5);

			pUnit->setUnitType(UnitType::COIN);
			currentCoinsOnScreen++;

			lastCoinFired = currentTime;
			pGame->setLastCoinFired(lastCoinFired);
		}

		pGame->setCoinsOnScreen(currentCoinsOnScreen);
	}

	//This is for spawning a powerUp
	double powerUpDelay = pGame->getPowerUpDelay();
	int powerUpFrequency = pGame->getPowerUpFrequency();
	double lastPowerUpFired = pGame->getLastPowerUpFired();
	double testPowerUpTime = powerUpDelay + lastPowerUpFired;

	if (testPowerUpTime <= currentTime && (rand() % powerUpFrequency == 0))
	{
		int currentPowerUpsOnScreen = pGame->getCurrentPowerUpsOnScreen();
		int maxPowerUpsOnScreen = pGame->getMaxPowerUpsOnScreen();
		if (maxPowerUpsOnScreen > currentPowerUpsOnScreen)
		{
			Grid* pGrid = pGame->getGrid();
			GridGraph* pGridGraph = pGame->getGridGraph();
			//Math for random grid location
			int posX = (rand() * 32) % pGame->getGraphicsSystem()->getWidth();
			int posY = (rand() * 32) % pGame->getGraphicsSystem()->getHeight();
			while (pGrid->getValueAtIndex(pGrid->getSquareIndexFromPixelXY(posX, posY)) == BLOCKING_VALUE)
			{
				posX = (rand() * 32) % gpGame->getGraphicsSystem()->getWidth();
				posY = (rand() * 32) % gpGame->getGraphicsSystem()->getHeight();
			}
			Unit* pUnit = gpGame->getUnitManager()->createUnit(*pGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
			pUnit->getPositionComponent()->setPosition(Vector2D(posX, posY));
			pUnit->setCollision(CIRCLE, 5);

			pUnit->setUnitType(UnitType::POWER_UP);
			currentPowerUpsOnScreen++;

			lastPowerUpFired = currentTime;
			pGame->setLastPowerUpFired(lastPowerUpFired);
		}

		pGame->setPowerUpsOnScreen(currentPowerUpsOnScreen);

	}
}