#include "Game.h"
#include "PlayerAiRunState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "../KinematicPlayerAiSteering.h"
#include "../SteeringFiles/Steering.h"
#include "../SteeringFiles/SteeringComponent.h"


using namespace std;

void PlayerAiRunState::onEntrance()
{
	mTransitionToAttack = false;
	mTransitionToPlayerControl = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID));

	Vector2D targetLoc = pPlayerUnit->getSteeringComponent()->getTargetLoc();
	int targetID = pPlayerUnit->getSteeringComponent()->getTargetID();
	pPlayerUnit->setSteering(Steering::KINEMATIC_PLAYER_AI, targetLoc, targetID);
}

void PlayerAiRunState::onExit()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->setLastPowerUpFired(gpGame->getCurrentTime());
}

StateTransition* PlayerAiRunState::update()
{
	if (mTransitionToAttack == true)
	{
		mTransitionToAttack = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_AI_TO_ATTACK_TRANSITION);
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

void PlayerAiRunState::transitionToPlayerControl()
{
	mTransitionToPlayerControl = true;
}

void PlayerAiRunState::transitionToAiAttack()
{
	mTransitionToAttack = true;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Sprite* pPlayerAttack = pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pPlayerAttack);
}