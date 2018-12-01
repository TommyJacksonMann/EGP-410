#include "PlayerMoveToMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "Vector2D.h"
#include "./SteeringFiles/UnitManager.h"

PlayerMoveToMessage::PlayerMoveToMessage(const Vector2D& destination)
	:GameMessage(PLAYER_MOVETO_MESSAGE)
	, mDestination(destination)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{
}

void PlayerMoveToMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		pGame->getUnitManager()->getPlayerUnit()->setSteering(Steering::KINEMATICARRIVE, mDestination);
	}
}