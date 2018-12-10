#include "SpawnEnemyMessage.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Game.h"
#include "SpriteManager.h"

SpawnEnemyMessage::SpawnEnemyMessage()
	:GameMessage(SPAWN_ENEMY_MESSAGE)
{}

void SpawnEnemyMessage::process()
{
	




	Unit* pEnemy = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID));
	pEnemy->getPositionComponent()->setPosition(Vector2D(10 * 32, 5 * 32));
	pEnemy->setCollision(CIRCLE, 5);
	pEnemy->setUnitType(UnitType::ENEMY);
	pEnemy->setSteering(Steering::KINEMATIC_ENEMY_ARRIVE, Vector2D(10 * 32, 5 * 32), gpGame->getUnitManager()->getPlayerUnit()->getID());
}