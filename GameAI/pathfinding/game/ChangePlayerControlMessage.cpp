#include "ChangePlayerControlMessage.h"
#include "GameMessage.h"
#include "Game.h"
#include ".\SteeringFiles\UnitManager.h"
#include ".\SteeringFiles\Unit.h"
#include <map>
#include "GameApp.h"
#include "./StateMachingFiles/StateMachine.h"
#include "./StateMachingFiles/PlayerAttackState.h"
#include "./StateMachingFiles/PlayerRunState.h"
#include "./StateMachingFiles/PlayerAiAttackState.h"
#include "./StateMachingFiles/PlayerAiRunState.h"

#include <typeinfo>

ChangePlayerControlMessage::ChangePlayerControlMessage()
	:GameMessage(CHANGE_PLAYER_CONTROL_TYPE_MESSAGE)
{
}


ChangePlayerControlMessage::~ChangePlayerControlMessage()
{
}


void ChangePlayerControlMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	StateMachineState* pCurrentPlayerState = pGame->getUnitManager()->getPlayerUnit()->getStateMachine()->getCurrentState();
	if (typeid(*pCurrentPlayerState) == typeid(PlayerRunState))
	{
		PlayerRunState* pState = static_cast<PlayerRunState*>(pCurrentPlayerState);
		pState->transitionToAiControl();
	}
	else if (typeid(*pCurrentPlayerState) == typeid(PlayerAttackState))
	{
		PlayerAttackState* pState = static_cast<PlayerAttackState*>(pCurrentPlayerState);
		pState->transitionToAiControl();
	}
	else if (typeid(*pCurrentPlayerState) == typeid(PlayerAiRunState))
	{
		PlayerAiRunState* pState = static_cast<PlayerAiRunState*>(pCurrentPlayerState);
		pState->transitionToPlayerControl();

	}
	else if (typeid(*pCurrentPlayerState) == typeid(PlayerAiAttackState))
	{
		PlayerAiAttackState* pState = static_cast<PlayerAiAttackState*>(pCurrentPlayerState);
		pState->transitionToPlayerControl();
	}
}
