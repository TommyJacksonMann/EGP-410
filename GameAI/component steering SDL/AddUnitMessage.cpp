#include "Game.h"
#include "GameMessageManager.h"
#include "AddUnitMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "GraphicsSystem.h"


AddUnitMessage::AddUnitMessage()
	:GameMessage(ADD_UNIT_MESSAGE)
{
}

AddUnitMessage::~AddUnitMessage()
{
}

void AddUnitMessage::process()
{
	Sprite* pEnemySprite = gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID);
	Unit* pUnit = gpGame->getUnitManager()->createUnit(*pEnemySprite, true, PositionData(Vector2D((float)gpGame->getGraphicsSystem()->getWidth() - 1, 0.0f), 0.0f));
	pUnit->setShowTarget(false);
	pUnit->setSteering(Steering::FLOCK, ZERO_VECTOR2D);
	int x = rand() % gpGame->getGraphicsSystem()->getWidth() - 1;
	int y = rand() % gpGame->getGraphicsSystem()->getHeight() - 1;
	pUnit->getPositionComponent()->setPosition(Vector2D(x, y));
}
