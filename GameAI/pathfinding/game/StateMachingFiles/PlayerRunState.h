#pragma once
#include "StateMachine.h"

/*
Class created by Tommy Mann

/**/
class PlayerRunState : public StateMachineState
{
public:
	PlayerRunState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToAttack();
	void transitionToAiControl();
private:
	bool mTransitionToAttack;
	bool mTransitionToAiControl;
};