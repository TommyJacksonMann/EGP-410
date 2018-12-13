#pragma once
#include "StateMachine.h"

/*
Class created by Tommy Mann

/**/
class AiWanderState : public StateMachineState
{
public:
	AiWanderState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToAiChase();
	void transitionToAiFlee();

private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
};