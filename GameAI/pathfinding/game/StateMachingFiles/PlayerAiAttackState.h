#pragma once
#include "StateMachine.h"

class PlayerAiAttackState : public StateMachineState
{
public:
	PlayerAiAttackState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToPlayerControl();
	void transitionToAiRun();
private:
	bool mTransitionToPlayerControl;
	bool mTransitionToRun;
};