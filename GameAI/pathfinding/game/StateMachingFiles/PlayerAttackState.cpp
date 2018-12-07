#include "Game.h"
#include "PlayerAttackState.h"
#include <iostream>

using namespace std;

void PlayerAttackState::onEntrance()
{
	cout << "\nEntering PlayerAttackState id:" << mID << endl;
	mTransitionToRun = false;
}

void PlayerAttackState::onExit()
{
	cout << "\nExitting PlayerAttackState id:" << mID << endl;
}

StateTransition* PlayerAttackState::update()
{
	cout << "PlayerAttackState!" << endl;

	if (mTransitionToRun == true)
	{
		mTransitionToRun = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_TO_RUN_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}


	return NULL;//no transition
}