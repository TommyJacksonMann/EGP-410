#include "Game.h"
#include "AiWanderState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "PlayerAttackState.h"
#include "PlayerAiAttackState.h"
#include "PlayerRunState.h"
#include "PlayerAiRunState.h"

using namespace std;

void AiWanderState::onEntrance()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwner);
	pOwner->setSteering(Steering::KINEMATIC_ENEMY_ARRIVE, pOwner->getPositionComponent()->getPosition(), gpGame->getUnitManager()->getPlayerUnit()->getID());

	mTransitionToChase = false;
	mTransitionToFlee = false;
}

void AiWanderState::onExit()
{
	mTransitionToChase = false;
	mTransitionToFlee = false;
}

StateTransition* AiWanderState::update()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	std::cout << "AI WANNNDDDDDEEEERRRRRRRRR\n";

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwner);
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();

	Vector2D diff = pOwner->getPositionComponent()->getPosition() - pPlayer->getPositionComponent()->getPosition();

	  if (diff.getLength() < pGame->getEnemyChaseRange())
	{
		if (typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAttackState) || typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAiAttackState))
		{
 			mTransitionToFlee = true;
		}
		else if (typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerRunState) || typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAiRunState))
		{
			mTransitionToChase = true;
		}
	}


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