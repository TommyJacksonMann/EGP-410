#pragma once
#include "GameMessage.h"

class AddUnitMessage :public GameMessage
{
public:
	AddUnitMessage();
	~AddUnitMessage();

	void process();
};