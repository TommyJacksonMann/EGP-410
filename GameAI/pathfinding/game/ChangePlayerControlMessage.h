#pragma once
#include "GameMessage.h"


class ChangePlayerControlMessage :
	public GameMessage
{
public:
	ChangePlayerControlMessage();
	~ChangePlayerControlMessage();

	void process();
};

