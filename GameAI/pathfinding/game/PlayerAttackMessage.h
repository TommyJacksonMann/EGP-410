#pragma once
#include "GameMessage.h"
#include "GameApp.h"
#include "Game.h"

class PlayerAttackMessage :public GameMessage
{
public:
	PlayerAttackMessage() :GameMessage(PLAYER_ATTACK_MESSAGE) {}
	~PlayerAttackMessage() {}

	void process();
};