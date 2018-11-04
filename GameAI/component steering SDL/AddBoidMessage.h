#pragma once
#include "GameMessage.h"

class AddBoidMessage :public GameMessage
{
public:
	AddBoidMessage();
	~AddBoidMessage();

	void process();
};