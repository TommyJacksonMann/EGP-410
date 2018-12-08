#include "Game.h"
#include "PlayerRunState.h"
#include <iostream>
#include "./Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void PlayerRunState::onEntrance()
{
	mTransitionToAttack = false;
}

void PlayerRunState::onExit()
{
}

StateTransition* PlayerRunState::update()
{

	//find the right transition
	if (mTransitionToAttack == true)
	{
		mTransitionToAttack = false;

		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_TO_ATTACK_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	
	return NULL;//no transition
}

void PlayerRunState::transitionToAttack()
{
	mTransitionToAttack = true;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Sprite* pPlayerAttack = pGame->getSpriteManager()->getSprite(PLAYER_ATTACK_ICON_SPRITE_ID);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pPlayerAttack);
}