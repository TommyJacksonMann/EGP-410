#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"

class PlayerRunMessage :public GameMessage
{
public:
	PlayerRunMessage() :GameMessage(PLAYER_RUN_MESSAGE) {}
	~PlayerRunMessage() {}

	void process();
};