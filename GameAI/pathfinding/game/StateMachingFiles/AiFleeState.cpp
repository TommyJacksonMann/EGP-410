#include "Game.h"
#include "AiFleeState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "PlayerRunState.h"
#include "PlayerAiRunState.h"
#include "../SteeringFiles/Steering.h"
#include "../SteeringFiles/SteeringComponent.h"

using namespace std;

void AiFleeState::onEntrance()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwner);
	pOwner->setSteering(Steering::KINEMATIC_ENEMY_RUN, pOwner->getSteeringComponent()->getTargetLoc(), gpGame->getUnitManager()->getPlayerUnit()->getID());

	mTransitionToChase = false;
	mTransitionToWander = false;
}

void AiFleeState::onExit()
{
	mTransitionToChase = false;
	mTransitionToWander = false;
}

StateTransition* AiFleeState::update()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwner);
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();

	Vector2D diff = pOwner->getPositionComponent()->getPosition() - pPlayer->getPositionComponent()->getPosition();

	if (diff.getLength() < pGame->getEnemyChaseRange())
	{
		if (typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerRunState) || typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAiRunState))
		{
			mTransitionToChase = true;
		}
	}
	else if (diff.getLength() > pGame->getEnemyChaseRange())
	{
		mTransitionToWander = true;

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

	if (mTransitionToWander == true)
	{
		mTransitionToWander = false;
		//find the right transition
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(AI_TO_WANDER_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;//no transition
}


void AiFleeState::transitionToAiChase()
{
	mTransitionToChase = true;
}

void AiFleeState::transitionToAiWander()
{
	mTransitionToWander = true;
}