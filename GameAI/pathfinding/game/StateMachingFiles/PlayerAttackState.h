#pragma once
#include "StateMachine.h"

class PlayerAttackState : public StateMachineState
{
public:
	PlayerAttackState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToRun();

private:
	bool mTransitionToRun;
};