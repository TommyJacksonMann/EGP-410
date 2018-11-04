#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"

class AStarMessage :public GameMessage
{
public:
	AStarMessage() : GameMessage(A_STAR_MESSAGE) {}
	~AStarMessage() {}

	void process()
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->SetPathFinderToAStar();
	}
};