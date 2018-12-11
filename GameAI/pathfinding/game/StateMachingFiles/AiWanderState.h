#pragma once
#include "StateMachine.h"

class AiWanderState : public StateMachineState
{
public:
	AiWanderState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToAiChase();
	void transitionToAiFlee();

private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
};