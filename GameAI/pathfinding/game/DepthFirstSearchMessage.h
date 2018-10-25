#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"

class DepthFirstSearchMessage :public GameMessage
{
public:
	DepthFirstSearchMessage() :GameMessage(DEPTH_FIRST_SEARCH_MESSAGE) {}
	~DepthFirstSearchMessage() {}

	void process()
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->SetPathFinderToDepthFirst();
	}
};