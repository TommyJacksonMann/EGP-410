#pragma once
#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

class PlayerMoveToMessage :public GameMessage
{
public:
	PlayerMoveToMessage(const Vector2D& destination);
	~PlayerMoveToMessage();

	void process();
private:
	Vector2D mDestination;
};