#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"

class DijkstraMessage :public GameMessage
{
public:
	DijkstraMessage() :GameMessage(DIJKSTRA_MESSAGE) {}
	~DijkstraMessage() {}

	void process()
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->SetPathFinderToDijkstra();
	}
};