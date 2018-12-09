#pragma once
#include "StateMachine.h"

class AiRunState : public StateMachineState
{
public:
	AiRunState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToPlayerControl();
	void transitionToAiAttack();
private:
	bool mTransitionToPlayerControl;
	bool mTransitionToAttack;
};