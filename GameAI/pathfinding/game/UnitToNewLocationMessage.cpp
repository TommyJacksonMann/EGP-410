#include "UnitToNewLocationMessage.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "./SteeringFiles/UnitManager.h"
#include <map>
#include "FollowPathSteering.h"
#include "./SteeringFiles/SteeringComponent.h"
#include "GridPathfinder.h"
#include "FlowFieldPathFinder.h"
#include "FlowFieldSteering.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "PathPool.h"
#include <typeinfo>

UnitToNewLocationMessage::UnitToNewLocationMessage(const Vector2D& pos)
:GameMessage(UNIT_TO_NEW_LOCATION_MESSAGE),
mPos(pos)
{
}


UnitToNewLocationMessage::~UnitToNewLocationMessage()
{
}

void UnitToNewLocationMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	if (!pGame->getUnitManager()->getUnitMap().empty())
	{
		std::map<UnitID, Unit*> tempMap = pGame->getUnitManager()->getUnitMap();
		std::map<UnitID, Unit*>::iterator iter = tempMap.begin();

		GridPathfinder* pPathfinder = pGame->getPathfinder();
		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		PathPool* pPathPool= pGame->getPathPool();
		while (iter != tempMap.end())
		{

			int fromIndex = pGrid->getSquareIndexFromPixelXY((int)iter->second->getPositionComponent()->getPosition().getX(), (int)iter->second->getPositionComponent()->getPosition().getY());
			int toIndex = pGrid->getSquareIndexFromPixelXY((int)mPos.getX(), (int)mPos.getY());
			
			

			if (typeid(*pGame->getPathfinder()) == typeid(FlowFieldPathfinder))
			{
				iter->second->setSteering(Steering::FLOW_FIELD);
				FlowFieldSteering* steer = static_cast<FlowFieldSteering*>(iter->second->getSteeringComponent()->getSteering());
				Node* pToNode = pGridGraph->getNode(fromIndex);
				steer->SetNode(pToNode);
			}
			else
			{
				iter->second->setSteering(Steering::FOLLOW_PATH);
				Path* tempPath = pPathPool->CheckPath(fromIndex, toIndex);
				FollowPathSteering* steer = static_cast<FollowPathSteering*>(iter->second->getSteeringComponent()->getSteering());
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
			
			
			++iter;
		}
	}
}
