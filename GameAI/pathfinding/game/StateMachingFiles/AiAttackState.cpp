#include "Game.h"
#include "AiAttackState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void AiAttackState::onEntrance()
{
	mTransitionToRun = false;
	mTransitionToPlayerControl = false;
}

void AiAttackState::onExit()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());
}

StateTransition* AiAttackState::update()
{
	std::cout << "AI ATTTTTTTTTAAAAAACKCKKKCKCKCKCKK\n";

	if (mTransitionToRun == true)
	{
		mTransitionToRun = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_RUN_TRANSITION);
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

void AiAttackState::transitionToPlayerControl()
{
	mTransitionToPlayerControl = true;
}

void AiAttackState::transitionToAiRun()
{
	mTransitionToRun = true;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Sprite* pPlayerAttack = pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pPlayerAttack);
}