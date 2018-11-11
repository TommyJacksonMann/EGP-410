#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"
#include "./SteeringFiles/UnitManager.h"

class FlowFieldMessage :public GameMessage
{
public:
	FlowFieldMessage() : GameMessage(A_STAR_MESSAGE) {}
	~FlowFieldMessage() {}

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
		pGame->SetPathFinderToFlowField();
	}
};