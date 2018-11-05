#include "AddAndRemoveUnitsMessage.h"
#include "GameMessage.h"
#include "Game.h"
#include ".\SteeringFiles\UnitManager.h"
#include ".\SteeringFiles\Unit.h"
#include <map>
#include "Sprite.h"
#include "SpriteManager.h"
#include "GameApp.h"
#include "InputSystem.h"


AddAndRemoveUnitsMessage::AddAndRemoveUnitsMessage()
	:GameMessage(ADD_AND_REMOVE_MESSAGE)
{
}


AddAndRemoveUnitsMessage::~AddAndRemoveUnitsMessage()
{
}


void AddAndRemoveUnitsMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Vector2D pos = pGame->getInputSystem()->getLastPosition();
	if (!gpGame->getUnitManager()->getUnitMap().empty())
	{
		while (gpGame->getUnitManager()->getUnitMap().size() > 0)
		{
			gpGame->getUnitManager()->deleteRandomUnit();
		}
	}
	Sprite* pSprite = gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID);
	bool testForPathExistence = false;
	if (pos == ZERO_VECTOR2D)
	{
		testForPathExistence = true;
	}
	for (int i = 0; i < 10; i++)
	{
		Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*pSprite);
		if(testForPathExistence)
		{
			pos = pUnit->getPositionComponent()->getPosition();
		}
		pUnit->setSteering(Steering::ARRIVE, Vector2D(pos.getX(), pos.getY()));
	}
}
