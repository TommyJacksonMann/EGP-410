#pragma once
#include "StateMachine.h"

/*
Class created by Tommy Mann

/**/
class PlayerAiAttackState : public StateMachineState
{
public:
	PlayerAiAttackState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToPlayerControl();
	void transitionToAiRun();
private:
	bool mTransitionToPlayerControl;
	bool mTransitionToRun;
};