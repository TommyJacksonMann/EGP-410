#pragma once
#include "GameMessage.h"
#include <typeinfo>


/*
Class created by Tommy Mann
Used to change the enemy AI's states
/**/

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

