#pragma once
#include "GameMessage.h"
#include <Vector2D.h>
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

