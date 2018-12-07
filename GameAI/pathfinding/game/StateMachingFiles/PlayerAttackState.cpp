#include "Game.h"
#include "PlayerAttackState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

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

void PlayerAttackState::transitionToRun()
{
	mTransitionToRun = true;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Sprite* pPlayerAttack = pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pPlayerAttack);
}