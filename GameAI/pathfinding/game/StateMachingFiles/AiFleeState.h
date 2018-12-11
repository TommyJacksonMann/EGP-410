#pragma once
#include "StateMachine.h"

class AiFleeState : public StateMachineState
{
public:
	AiFleeState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToAiChase();
	void transitionToAiWander();

private:
	bool mTransitionToChase;
	bool mTransitionToWander;
};