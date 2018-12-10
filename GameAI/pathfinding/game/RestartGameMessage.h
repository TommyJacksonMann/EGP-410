#pragma once
#include "GameMessage.h"
class RestartGameMessage :
	public GameMessage
{
public:
	RestartGameMessage();
	~RestartGameMessage();

	void process();
};

