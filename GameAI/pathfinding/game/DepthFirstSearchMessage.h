#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"
#include "./SteeringFiles/UnitManager.h"

class DepthFirstSearchMessage :public GameMessage
{
public:
	DepthFirstSearchMessage() :GameMessage(DEPTH_FIRST_SEARCH_MESSAGE) {}
	~DepthFirstSearchMessage() {}

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
		pGame->SetPathFinderToDepthFirst();
	}
};