#include "UnitToNewLocationMessage.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "./SteeringFiles/UnitManager.h"
#include <map>
#include "FollowPathSteering.h"
#include "./SteeringFiles/SteeringComponent.h"
#include "GridPathfinder.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"

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

		while (iter != tempMap.end())
		{
			iter->second->setSteering(Steering::FOLLOW_PATH);
			FollowPathSteering* steer = static_cast<FollowPathSteering*>(iter->second->getSteeringComponent()->getSteering());
			GridPathfinder* pPathfinder = pGame->getPathfinder();
			GridGraph* pGridGraph = pGame->getGridGraph();
			Grid* pGrid = pGame->getGrid();
			int fromIndex = pGrid->getSquareIndexFromPixelXY((int)iter->second->getPositionComponent()->getPosition().getX(), (int)iter->second->getPositionComponent()->getPosition().getY());
			int toIndex = pGrid->getSquareIndexFromPixelXY((int)mPos.getX(), (int)mPos.getY());
			Node* pFromNode = pGridGraph->getNode(fromIndex);
			Node* pToNode = pGridGraph->getNode(toIndex);			
			steer->SetPath(pPathfinder->findPath(pFromNode, pToNode));
			++iter;
		}
	}
}
