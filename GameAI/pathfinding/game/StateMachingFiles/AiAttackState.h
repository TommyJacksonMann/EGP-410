#pragma once
#include "StateMachine.h"

class AiAttackState : public StateMachineState
{
public:
	AiAttackState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToPlayerControl();
	void transitionToAiRun();
private:
	bool mTransitionToPlayerControl;
	bool mTransitionToRun;
};