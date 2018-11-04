#include "Game.h"
#include "GameMessageManager.h"
#include "AddBoidMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "GraphicsSystem.h"

AddBoidMessage::AddBoidMessage()
	:GameMessage(ADD_BOID_MESSAGE)
{
}

AddBoidMessage::~AddBoidMessage()
{
}

void AddBoidMessage::process()
{
	Sprite* pEnemySprite = gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID);
	
	int centerX = rand() % gpGame->getGraphicsSystem()->getWidth() - 1;
	int centerY = rand() % gpGame->getGraphicsSystem()->getHeight() - 1;

	for (int i = 0; i < 10; i++)
	{
		Unit* pUnit = gpGame->getUnitManager()->createUnit(*pEnemySprite, true, PositionData(Vector2D((float)gpGame->getGraphicsSystem()->getWidth() - 1, 0.0f), 0.0f));
		pUnit->setShowTarget(false);
		pUnit->setSteering(Steering::FLOCK, ZERO_VECTOR2D);
		Vector2D offset(rand() % (int)DEFAULT_NEIGHBORHOOD_RADIUS, rand() % (int)DEFAULT_NEIGHBORHOOD_RADIUS);
		offset.normalize();
		offset *= rand() % ((int)DEFAULT_NEIGHBORHOOD_RADIUS*2);
		pUnit->getPositionComponent()->setPosition(Vector2D(centerX + offset.getX(), centerY + offset.getY()));
	}
}
