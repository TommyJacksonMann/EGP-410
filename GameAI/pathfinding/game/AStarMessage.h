#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"
#include "./SteeringFiles/UnitManager.h"

class AStarMessage :public GameMessage
{
public:
	AStarMessage() : GameMessage(A_STAR_MESSAGE) {}
	~AStarMessage() {}

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
		pGame->SetPathFinderToAStar();
	}
};