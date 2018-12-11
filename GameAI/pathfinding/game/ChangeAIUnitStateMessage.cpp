#include "ChangeAIUnitStateMessage.h"
#include "GameMessage.h"
#include "Game.h"
#include ".\SteeringFiles\UnitManager.h"
#include ".\SteeringFiles\Unit.h"
#include <map>
#include "GameApp.h"
#include "./StateMachingFiles/StateMachine.h"
#include "./SteeringFiles/UnitManager.h"
#include "./StateMachingFiles/AiWanderState.h"
#include "./StateMachingFiles/AiChaseState.h"
#include "./StateMachingFiles/AiFleeState.h"


ChangeAIUnitStateMessage::ChangeAIUnitStateMessage(int id, StateType stateType)
	:GameMessage(CHANGE_AI_UNIT_STATE_MESSAGE)
	, mUnitID(id)
	, mStateType(stateType)
{
}


ChangeAIUnitStateMessage::~ChangeAIUnitStateMessage()
{
}


void ChangeAIUnitStateMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	StateMachineState* pCurrentState = pGame->getUnitManager()->getUnit(mUnitID)->getStateMachine()->getCurrentState();
	if (typeid(*pCurrentState) == typeid(AiWanderState))
	{
		
		AiWanderState* pState = static_cast<AiWanderState*>(pCurrentState);
		if (mStateType == StateType::CHASE)
		{
			pState->transitionToAiChase();
		}
		else if (mStateType == StateType::FLEE)
		{
			pState->transitionToAiFlee();
		}
	}
	else if (typeid(*pCurrentState) == typeid(AiFleeState))
	{
		AiFleeState* pState = static_cast<AiFleeState*>(pCurrentState);
		if (mStateType == StateType::CHASE)
		{
			pState->transitionToAiChase();
		}
		else if (mStateType == StateType::WANDER)
		{
			pState->transitionToAiWander();
		}
	}
	else if (typeid(*pCurrentState) == typeid(AiChaseState))
	{
		AiChaseState* pState = static_cast<AiChaseState*>(pCurrentState);
		if (mStateType == StateType::WANDER)
		{
			pState->transitionToAiWander();
		}
		else if (mStateType == StateType::FLEE)
		{
			pState->transitionToAiFlee();
		}

	}
}
