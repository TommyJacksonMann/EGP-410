#include "KinematicEnemySteering.h"
#include "Game.h"
#include "GameApp.h"


KinematicEnemySteering::KinematicEnemySteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::KINEMATIC_ENEMY_ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mMovementFactor = pGame->getPlayerSpeed();
}

Steering* KinematicEnemySteering::getSteering()
{
	return NULL;
}