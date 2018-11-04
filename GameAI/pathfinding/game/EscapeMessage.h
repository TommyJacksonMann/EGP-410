#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "../common/Game.h"

class EscapeMessage :public GameMessage
{
public:
	EscapeMessage() :GameMessage(ESCAPE_MESSAGE) {}
	~EscapeMessage(){}

	void process()
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->markForExit();
	}
};