#pragma once
#include "GameMessage.h"
#include <typeinfo>

enum StateType{
	WANDER,
	CHASE,
	FLEE
};

class ChangeAIUnitStateMessage :
	public GameMessage
{
public:
	ChangeAIUnitStateMessage(int id, StateType stateType);
	~ChangeAIUnitStateMessage();

	void process();

private:
	int mUnitID;
	StateType  mStateType;
};

