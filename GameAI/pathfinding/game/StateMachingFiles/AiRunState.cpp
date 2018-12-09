#include "Game.h"
#include "AiRunState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

void AiRunState::onEntrance()
{
	mTransitionToAttack = false;
	mTransitionToPlayerControl = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID));
}

void AiRunState::onExit()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());
}

StateTransition* AiRunState::update()
{
	std::cout << "AI RUUUUUUUUUNNNNNNNNNN\n";

	if (mTransitionToAttack == true)
	{
		mTransitionToAttack = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_ATTACK_TRANSITION);
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
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_TO_RUN_TRANSITION);
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

void AiRunState::transitionToPlayerControl()
{
	mTransitionToPlayerControl = true;
}

void AiRunState::transitionToAiAttack()
{
	mTransitionToAttack = true;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Sprite* pPlayerAttack = pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pPlayerAttack);
}