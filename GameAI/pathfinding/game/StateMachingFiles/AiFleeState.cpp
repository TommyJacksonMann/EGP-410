#include "Game.h"
#include "AiFleeState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void AiFleeState::onEntrance()
{
	mTransitionToChase = false;
	mTransitionToWander = false;
}

void AiFleeState::onExit()
{
	
}

StateTransition* AiFleeState::update()
{
	std::cout << "AI FFFFFFFFFLLLLLLLLLLLLLLLLEEEEEEEEEEEEEEEE\n";

	if (mTransitionToChase == true)
	{
		mTransitionToChase = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_CHASE_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	if (mTransitionToWander == true)
	{
		mTransitionToWander = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_WANDER_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;//no transition
}


void AiFleeState::transitionToAiChase()
{
	mTransitionToChase = true;
}

void AiFleeState::transitionToAiWander()
{
	mTransitionToWander = true;
}