#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"
#include "./SteeringFiles/UnitManager.h"

class DijkstraMessage :public GameMessage
{
public:
	DijkstraMessage() :GameMessage(DIJKSTRA_MESSAGE) {}
	~DijkstraMessage() {}

	void process()
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		if (!pGame->getUnitManager()->getUnitMap().empty())
		{
			while (pGame->getUnitManager()->getUnitMap().size() > 0)
			{
				pGame->getUnitManager()->deleteRandomUnit();
			}
		}
		pGame->SetPathFinderToDijkstra();
	}
};