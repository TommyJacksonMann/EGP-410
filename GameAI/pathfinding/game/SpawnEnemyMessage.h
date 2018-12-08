#pragma once
#include "GameMessage.h"

class SpawnEnemyMessage
	: public GameMessage
{
public:
	SpawnEnemyMessage();
	~SpawnEnemyMessage() {}

	void process();
};
