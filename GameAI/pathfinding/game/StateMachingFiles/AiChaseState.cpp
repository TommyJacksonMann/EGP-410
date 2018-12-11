#include "Game.h"
#include "AiChaseState.h"
#include <iostream>
#include "Game.h"
#include "../GameApp.h"
#include "../SteeringFiles/UnitManager.h"
#include "../SteeringFiles/Unit.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "PlayerAttackState.h"
#include "PlayerAiAttackState.h"
#include "../SteeringFiles/Steering.h"
#include "../SteeringFiles/SteeringComponent.h"

using namespace std;

void AiChaseState::onEntrance()
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwner);
	pOwner->setSteering(Steering::KINEMATIC_ENEMY_CHASE, pOwner->getSteeringComponent()->getTargetLoc(), gpGame->getUnitManager()->getPlayerUnit()->getID());


	mTransitionToWander = false;
	mTransitionToFlee = false;
}

void AiChaseState::onExit()
{
	mTransitionToWander = false;
	mTransitionToFlee = false;
}

StateTransition* AiChaseState::update()
{


	GameApp* pGame = static_cast<GameApp*>(gpGame);

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwner);
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();

	Vector2D diff = pOwner->getPositionComponent()->getPosition() - pPlayer->getPositionComponent()->getPosition();

	if (diff.getLength() < pGame->getEnemyChaseRange())
	{
		if (typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAttackState) || typeid(*pPlayer->getStateMachine()->getCurrentState()) == typeid(PlayerAiAttackState))
		{
			mTransitionToFlee = true;
		}
	}
	else if (diff.getLength() > pGame->getEnemyChaseRange())
	{
		mTransitionToWander = true;
	
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


void AiChaseState::transitionToAiWander()
{
	mTransitionToWander = true;
}

void AiChaseState::transitionToAiFlee()
{
	mTransitionToFlee = true;
}