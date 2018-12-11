#include "Game.h"
#include "AiWanderState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void AiWanderState::onEntrance()
{
	mTransitionToChase = false;
	mTransitionToFlee = false;
}

void AiWanderState::onExit()
{

}

StateTransition* AiWanderState::update()
{
	std::cout << "AI WANNNDDDDDEEEERRRRRRRRR\n";

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


void AiWanderState::transitionToAiChase()
{
	mTransitionToChase = true;
}

void AiWanderState::transitionToAiFlee()
{
	mTransitionToFlee = true;
}