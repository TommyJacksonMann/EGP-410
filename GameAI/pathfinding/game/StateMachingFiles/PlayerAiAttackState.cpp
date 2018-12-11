#include "Game.h"
#include "PlayerAiAttackState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void PlayerAiAttackState::onEntrance()
{
	mTransitionToRun = false;
	mTransitionToPlayerControl = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pGame->getSpriteManager()->getSprite(PLAYER_ATTACK_ICON_SPRITE_ID));
}

void PlayerAiAttackState::onExit()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());
}

StateTransition* PlayerAiAttackState::update()
{
	if (mTransitionToRun == true)
	{
		mTransitionToRun = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_AI_TO_RUN_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToPlayerControl == true)
	{
		mTransitionToPlayerControl = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_TO_ATTACK_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());

	return NULL;//no transition
}

void PlayerAiAttackState::transitionToPlayerControl()
{
	mTransitionToPlayerControl = true;
}

void PlayerAiAttackState::transitionToAiRun()
{
	mTransitionToRun = true;
}