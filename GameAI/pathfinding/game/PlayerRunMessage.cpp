#include "PlayerRunMessage.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "./StateMachingFiles/StateMachine.h"
#include "./StateMachingFiles/PlayerAttackState.h"
#include "./StateMachingFiles/PlayerRunState.h"
#include "./StateMachingFiles/PlayerAiAttackState.h"
#include <typeinfo>

void PlayerRunMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	StateMachineState* pCurrentState = pGame->getUnitManager()->getPlayerUnit()->getStateMachine()->getCurrentState();

	if (typeid(*pCurrentState) == typeid(PlayerAttackState))
	{
		PlayerAttackState* pAttackState = static_cast<PlayerAttackState*>(pCurrentState);
		pAttackState->transitionToRun();
	}
	else if (typeid(*pCurrentState) == typeid(PlayerAiAttackState))
	{
		PlayerAiAttackState* pAttackState = static_cast<PlayerAiAttackState*>(pCurrentState);
		pAttackState->transitionToAiRun();
	}
}