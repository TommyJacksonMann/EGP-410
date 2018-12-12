#include "Game.h"
#include "PlayerRunState.h"
#include <iostream>
#include "./Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"

#include "../KinematicPlayerAiSteering.h"
#include "../SteeringFiles/Steering.h"
#include "../SteeringFiles/SteeringComponent.h"

using namespace std;

void PlayerRunState::onEntrance()
{
	mTransitionToAttack = false;
	mTransitionToAiControl = false;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pPlayerUnit = pGame->getUnitManager()->getPlayerUnit();
	pPlayerUnit->setSprite(*pGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID));

	Vector2D targetLoc = pPlayerUnit->getSteeringComponent()->getTargetLoc();
	int targetID = pPlayerUnit->getSteeringComponent()->getTargetID();
	pPlayerUnit->setSteering(Steering::KINEMATICARRIVE, targetLoc, targetID);

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
	if (mTransitionToAiControl == true)
	{
		mTransitionToAiControl = false;

		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_AI_TO_RUN_TRANSITION);
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
	
}

void PlayerRunState::transitionToAiControl()
{
	mTransitionToAiControl = true;
}