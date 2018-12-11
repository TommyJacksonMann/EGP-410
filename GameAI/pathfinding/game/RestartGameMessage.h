#pragma once
#include "GameMessage.h"
class RestartGameMessage :
	public GameMessage
{
public:
	RestartGameMessage(bool restartGame = false);
	~RestartGameMessage();

	void process();
private:
	bool mResetOrRestart;
};

