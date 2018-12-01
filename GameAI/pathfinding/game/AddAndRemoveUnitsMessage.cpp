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
#include "FlowFieldSteering.h"
#include "FlowFieldPathFinder.h"
#include "./SteeringFiles/SteeringComponent.h"
#include "GridPathfinder.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "PathPool.h"
#include <typeinfo>

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
		while (gpGame->getUnitManager()->getUnitMap().size() > 1)
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
	PathPool* pPathPool = pGame->getPathPool();
	for (int i = 0; i < 10; i++)
	{
		Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*pSprite);
		if (testForPathExistence)
		{
			pos = pUnit->getPositionComponent()->getPosition();
		}
		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)pUnit->getPositionComponent()->getPosition().getX(), (int)pUnit->getPositionComponent()->getPosition().getY());
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)pos.getX(), (int)pos.getY());
		
		if (typeid(*pGame->getPathfinder()) == typeid(FlowFieldPathfinder))
		{
			pUnit->setSteering(Steering::FLOW_FIELD);
			FlowFieldSteering* steer = static_cast<FlowFieldSteering*>(pUnit->getSteeringComponent()->getSteering());
			Node* pToNode = pGridGraph->getNode(fromIndex);
			steer->SetNode(pToNode);
		}
		else
		{
			pUnit->setSteering(Steering::FOLLOW_PATH);
			Path* tempPath = pPathPool->CheckPath(fromIndex, toIndex);
			FollowPathSteering* steer = static_cast<FollowPathSteering*>(pUnit->getSteeringComponent()->getSteering());

			if (tempPath)
			{
				steer->SetPath(tempPath);
			}
			else
			{
				Node* pFromNode = pGridGraph->getNode(fromIndex);
				Node* pToNode = pGridGraph->getNode(toIndex);
				tempPath = pPathfinder->findPath(pFromNode, pToNode);
				steer->SetPath(tempPath);
				pPathPool->AddPath(fromIndex, toIndex, tempPath);

			}
		}

		
	}

	
	
	
}
