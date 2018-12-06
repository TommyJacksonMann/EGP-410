#pragma once

#include "GameMessage.h"


//Class made by RJ, Resets player position, steering, and collision
//Can be put in the restart function
class ResetPlayerStartPos : public GameMessage
{
public:
	ResetPlayerStartPos();
	~ResetPlayerStartPos() {}

	void process();
};