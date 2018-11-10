#pragma once
#include "GameMessage.h"
class AddAndRemoveUnitsMessage :
	public GameMessage
{
public:
	AddAndRemoveUnitsMessage();
	~AddAndRemoveUnitsMessage();

	void process();
};

