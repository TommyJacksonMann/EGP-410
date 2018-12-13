#pragma once
#include "StateMachine.h"

/*
Class created by Tommy Mann

/**/
class PlayerAiRunState : public StateMachineState
{
public:
	PlayerAiRunState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToPlayerControl();
	void transitionToAiAttack();
private:
	bool mTransitionToPlayerControl;
	bool mTransitionToAttack;
};