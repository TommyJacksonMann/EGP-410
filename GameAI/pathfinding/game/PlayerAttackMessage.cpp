#include "PlayerAttackMessage.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "./StateMachingFiles/StateMachine.h"
#include "./StateMachingFiles/PlayerAttackState.h"
#include "./StateMachingFiles/PlayerRunState.h"
#include "./StateMachingFiles/AiAttackState.h"
#include "./StateMachingFiles/AiRunState.h"
#include <typeinfo>

void PlayerAttackMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	StateMachineState* pCurrentState = pGame->getUnitManager()->getPlayerUnit()->getStateMachine()->getCurrentState();
	if (typeid(*pCurrentState) == typeid(PlayerRunState))
	{
		PlayerRunState* pAttackState = static_cast<PlayerRunState*>(pCurrentState);
		pAttackState->transitionToAttack();
	}
	else if (typeid(*pCurrentState) == typeid(AiRunState))
	{
		AiRunState* pAttackState = static_cast<AiRunState*>(pCurrentState);
		pAttackState->transitionToAiAttack();
	}
}