#pragma once

#include "GameMessage.h"

class DeleteAllCoinsMessage : public  GameMessage
{
public :
	DeleteAllCoinsMessage();
	~DeleteAllCoinsMessage() {}

	void process();

};