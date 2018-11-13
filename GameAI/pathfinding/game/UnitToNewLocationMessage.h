#pragma once
#include "GameMessage.h"
#include <Vector2D.h>

//Changes the target node of the pathfinding for every Unit. Created by RJ Bourdelais

class UnitToNewLocationMessage :
	public GameMessage
{
public:

	UnitToNewLocationMessage(const Vector2D& pos);
	~UnitToNewLocationMessage();

	void process();
private:
	Vector2D mPos;
};

