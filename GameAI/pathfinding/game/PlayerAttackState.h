#pragma once
#include "./StateMachingFiles/StateMachine.h"

class PlayerAttackState : public StateMachineState
{
public:
	PlayerAttackState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
private:
};