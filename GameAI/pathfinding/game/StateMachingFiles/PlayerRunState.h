#pragma once
#include "StateMachine.h"

class PlayerRunState : public StateMachineState
{
public:
	PlayerRunState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
private:
	bool mTransitionToAttack;
};