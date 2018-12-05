#pragma once

#include "GameMessage.h"

class SpawnCoinMessage : public GameMessage
{
public:
	SpawnCoinMessage();
	~SpawnCoinMessage();

	void process();
};