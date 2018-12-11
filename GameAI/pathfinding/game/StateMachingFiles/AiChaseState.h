#pragma once
#include "StateMachine.h"

class AiChaseState : public StateMachineState
{
public:
	AiChaseState(const SM_idType& id, const UnitID owner) :StateMachineState(id, owner) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToAiWander();
	void transitionToAiFlee();

private:
	bool mTransitionToWander;
	bool mTransitionToFlee;

};