#pragma once

#include "GameMessage.h"

/*
* Class implemnation by RJ
* Fires every frame (probably not good) and checks to see if a new coin needs to be spawned
* this is with a frequency and time delay
* Also fires PowerUps
*/

class SpawnCoinMessage : public GameMessage
{
public:
	SpawnCoinMessage();
	~SpawnCoinMessage();

	void process();
};