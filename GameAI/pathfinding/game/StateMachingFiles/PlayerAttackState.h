#pragma once
#include "StateMachine.h"

class PlayerAttackState : public StateMachineState
{
public:
	PlayerAttackState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToRun();
	void transitionToAiControl();
private:
	bool mTransitionToRun;
	bool mTransitionToAiControl;
};