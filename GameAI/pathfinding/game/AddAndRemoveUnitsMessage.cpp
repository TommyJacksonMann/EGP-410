#include "AddAndRemoveUnitsMessage.h"
#include "GameMessage.h"
#include "Game.h"
#include ".\SteeringFiles\UnitManager.h"
#include ".\SteeringFiles\Unit.h"
#include <map>
#include "Sprite.h"
#include "SpriteManager.h"
#include "GameApp.h"
#include "InputSystem.h"
#include "FollowPathSteering.h"
#include "./SteeringFiles/SteeringComponent.h"
#include "GridPathfinder.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"

AddAndRemoveUnitsMessage::AddAndRemoveUnitsMessage()
	:GameMessage(ADD_AND_REMOVE_MESSAGE)
{
}


AddAndRemoveUnitsMessage::~AddAndRemoveUnitsMessage()
{
}


void AddAndRemoveUnitsMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Vector2D pos = pGame->getInputSystem()->getLastPosition();
	if (!gpGame->getUnitManager()->getUnitMap().empty())
	{
		while (gpGame->getUnitManager()->getUnitMap().size() > 0)
		{
			gpGame->getUnitManager()->deleteRandomUnit();
		}
	}
	Sprite* pSprite = gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID);
	bool testForPathExistence = false;

	if (pos == ZERO_VECTOR2D)
	{
		testForPathExistence = true;
	}

	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	for (int i = 0; i < 10; i++)
	{
		Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*pSprite);
		if(testForPathExistence)
		{
			pos = pUnit->getPositionComponent()->getPosition();
		}
		pUnit->setSteering(Steering::FOLLOW_PATH);
		FollowPathSteering* steer = static_cast<FollowPathSteering*>(pUnit->getSteeringComponent()->getSteering());

		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)pUnit->getPositionComponent()->getPosition().getX(), (int)pUnit->getPositionComponent()->getPosition().getY());
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)pos.getX(), (int)pos.getY());
		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);
		steer->SetPath(pPathfinder->findPath(pFromNode, pToNode));
	}
}
