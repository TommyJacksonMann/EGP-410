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
	mTransitionToRun = false;
	mTransitionToAiControl = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pGame->getSpriteManager()->getSprite(PLAYER_ATTACK_ICON_SPRITE_ID));
}

void PlayerAttackState::onExit()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());
}

StateTransition* PlayerAttackState::update()
{
	std::cout << "PLAYER AAAAAAATTTTTTTTAAAAACCCCCKKKKK\n";

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
	if (mTransitionToAiControl == true)
	{
		mTransitionToAiControl = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_ATTACK_TRANSITION);
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

void PlayerAttackState::transitionToRun()
{
	mTransitionToRun = true;
}

void PlayerAttackState::transitionToAiControl()
{
	mTransitionToAiControl = true;
}