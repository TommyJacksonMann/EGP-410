#include "Game.h"
#include "AiChaseState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void AiChaseState::onEntrance()
{
	mTransitionToWander = false;
	mTransitionToFlee = false;
}

void AiChaseState::onExit()
{

}

StateTransition* AiChaseState::update()
{
	std::cout << "AI CHAAAAASSSSSEEEEEEEEEE\n";

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
	if (mTransitionToFlee == true)
	{
		mTransitionToFlee = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_FLEE_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;//no transition
}


void AiChaseState::transitionToAiWander()
{
	mTransitionToWander = true;
}

void AiChaseState::transitionToAiFlee()
{
	mTransitionToFlee = true;
}