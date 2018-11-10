#include "Game.h"
#include "GameMessageManager.h"
#include "DeleteUnitMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "GraphicsSystem.h"


DeleteUnitMessage::DeleteUnitMessage()
	:GameMessage(ADD_UNIT_MESSAGE)
{
}

DeleteUnitMessage::~DeleteUnitMessage()
{
}

void DeleteUnitMessage::process()
{
	gpGame->getUnitManager()->deleteRandomUnit();
}
