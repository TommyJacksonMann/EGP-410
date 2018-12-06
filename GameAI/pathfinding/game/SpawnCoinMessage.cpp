#include "SpawnCoinMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles//UnitManager.h"
#include "SpriteManager.h"
#include "GridGraph.h"
#include "Grid.h"
#include "GraphicsSystem.h"
#include "./SteeringFiles/PositionComponent.h"


SpawnCoinMessage::SpawnCoinMessage()
	: GameMessage(SPAWN_COIN_MESSAGE)
{

}

SpawnCoinMessage::~SpawnCoinMessage()
{

}

void SpawnCoinMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	int currentCoinsOnScreen = pGame->getCurrentCoinsOnScreen();
	int maxCoinsOnScreen = pGame->getMaxCoinsOnScreen();
	while (maxCoinsOnScreen > currentCoinsOnScreen)
	{
		Grid* pGrid = pGame->getGrid();
		GridGraph* pGridGraph = pGame->getGridGraph();
		//Math for random grid location
		int posX = (rand()*32) % pGame->getGraphicsSystem()->getWidth();
		int posY = (rand()*32) % pGame->getGraphicsSystem()->getHeight();
		while (pGrid->getValueAtIndex(pGrid->getSquareIndexFromPixelXY(posX, posY)) == BLOCKING_VALUE)
		{
			posX = (rand()* 32) % gpGame->getGraphicsSystem()->getWidth();
			posY = (rand() * 32) % gpGame->getGraphicsSystem()->getHeight();
		}
		Unit* pUnit = gpGame->getUnitManager()->createUnit(*pGame->getSpriteManager()->getSprite(COIN_SPRITE_ID));
		pUnit->getPositionComponent()->setPosition(Vector2D(posX, posY));
		pUnit->setCollision(CIRCLE, 5);

		pUnit->setUnitType(UnitType::COIN);
		currentCoinsOnScreen++;
	}

	if (maxCoinsOnScreen == currentCoinsOnScreen)
	{
		pGame->setCoinsOnScreen(currentCoinsOnScreen);
	}
}