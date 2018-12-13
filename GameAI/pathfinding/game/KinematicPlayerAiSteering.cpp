#include "KinematicPlayerAiSteering.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Grid.h"
#include "GridGraph.h"
#include <cassert>
#include "PathPool.h"
#include "GridPathfinder.h"
#include "StateMachingFiles/StateMachine.h"
#include "StateMachingFiles/PlayerAiAttackState.h"
#include "StateMachingFiles/PlayerAiRunState.h"
#include <typeinfo>



KinematicPlayerAiSteering::KinematicPlayerAiSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::KINEMATIC_PLAYER_AI;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mMovementFactor = pGame->getPlayerSpeed();

	determineDestination();
	updatePath();
}

void KinematicPlayerAiSteering::updatePath()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetPlayerPos = pTarget->getPositionComponent()->getPosition();
	}

	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	PathPool* pPathPool = pGame->getPathPool();

	Vector2D unitPos = pOwner->getPositionComponent()->getPosition();
	unitPos += Vector2D(16, 16);

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)unitPos.getX(), (int)unitPos.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)mTargetPlayerPos.getX(), (int)mTargetPlayerPos.getY());

	Path* tempPath = pPathPool->CheckPath(fromIndex, toIndex);

	if (tempPath)
	{
		mPath = *tempPath;
	}
	else
	{
		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);
		tempPath = pPathfinder->findPath(pFromNode, pToNode);
		mPath = *tempPath;
		pPathPool->AddPath(fromIndex, toIndex, tempPath);
	}
	mCurrentPathPosition = 1;
	mJustChangedPath = true;
}


Steering* KinematicPlayerAiSteering::getSteering()
{

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetPlayerPos = pTarget->getPositionComponent()->getPosition();
	}

	if (mCurrentPathPosition > mPath.getNumNodes() || !mPath.peekNode(mCurrentPathPosition))
	{
		determineDestination();
		updatePath();
		return this;
	}

	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D currentTargetNodePosition = pGrid->getULCornerOfSquare(mPath.peekNode(mCurrentPathPosition)->getId());

	//Get the difference of the node position and the unit position
	Vector2D diff = currentTargetNodePosition - pOwner->getPositionComponent()->getPosition();

	if (diff.getLength() < mMovementFactor)
	{
		if (mCurrentPathPosition == mPath.getNumNodes() - 1)
		{
			data.vel = ZERO_VECTOR2D;
			data.acc = ZERO_VECTOR2D;
			data.rotAcc = 0;
			data.rotVel = 0;
			determineDestination();
			updatePath();
		}
		else
		{
			mCurrentPathPosition++;
			setTargetLoc(pGrid->getULCornerOfSquare(mPath.peekNode(mCurrentPathPosition)->getId()));
			/*if (!mJustChangedPath)
			{*/
			updatePath();
			//}
		}

	}
	else
	{
		float targetSpeed;
		Vector2D targetVelocity;
		diff.normalize();

		targetVelocity = diff;
		targetVelocity *= mMovementFactor;
		pOwner->getPositionComponent()->setPosition(pOwner->getPositionComponent()->getPosition() + targetVelocity);
		float velocityDirection = atan2(diff.getY(), diff.getX()) + .5f*3.14;
		pOwner->getPositionComponent()->setFacing(velocityDirection);
	}

	this->mData = data;
	return this;

}

void KinematicPlayerAiSteering::determineDestination()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	std::vector<Unit*> coinUnits = gpGame->getUnitManager()->getUnitsOfType(UnitType::COIN);
	std::vector<Unit*> powerupUnits = gpGame->getUnitManager()->getUnitsOfType(UnitType::POWER_UP);
	std::vector<Unit*> enemyUnits = gpGame->getUnitManager()->getUnitsOfType(UnitType::ENEMY);


	bool foundNewDestination = false;
	
	if (typeid(*pOwner->getStateMachine()->getCurrentState()) == typeid(PlayerAiAttackState) && enemyUnits.size() != 0)
	{
		int nearestEnemyIndex = 0;
		float shortestDist = 1000000; //high number because INFINITY doesn't work
		for (int i = 0; i < enemyUnits.size(); i++)
		{
			Vector2D currentDiff = enemyUnits[i]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			Vector2D prevDiff = enemyUnits[nearestEnemyIndex]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			if (currentDiff.getLength() <= prevDiff.getLength() && enemyUnits[nearestEnemyIndex]->getID() != mTargetID)
			{
				foundNewDestination = true;
				shortestDist = currentDiff.getLength();
				nearestEnemyIndex = i;
			}
		}
		setTargetID(enemyUnits[nearestEnemyIndex]->getID());
		setTargetLoc(enemyUnits[nearestEnemyIndex]->getPositionComponent()->getPosition());
	}
	else if (powerupUnits.size() != 0)
	{
		int nearestPowerUpIndex = 0;
		float shortestDist = 1000000; //high number because INFINITY doesn't work
		for (int i = 0; i < powerupUnits.size(); i++)
		{
			Vector2D currentDiff = powerupUnits[i]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			Vector2D prevDiff = powerupUnits[nearestPowerUpIndex]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			if (currentDiff.getLength() <= prevDiff.getLength() && powerupUnits[nearestPowerUpIndex]->getID() != mTargetID)
			{
				foundNewDestination = true;
				shortestDist = currentDiff.getLength();
				nearestPowerUpIndex = i;
			}
		}
		setTargetID(powerupUnits[nearestPowerUpIndex]->getID());
		setTargetLoc(powerupUnits[nearestPowerUpIndex]->getPositionComponent()->getPosition());
	}
	else if (coinUnits.size() != 0)
	{
		int nearestCoinIndex = 0;
		float shortestDist = 1000000; //high number because INFINITY DOESN'T WORK
		for (int i = 0; i < coinUnits.size(); i++)
		{
			Vector2D currentDiff = coinUnits[i]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			Vector2D prevDiff = coinUnits[nearestCoinIndex]->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			if (currentDiff.getLength() <= prevDiff.getLength() && coinUnits[nearestCoinIndex]->getID() != mTargetID)
			{
				foundNewDestination = true;
				shortestDist = currentDiff.getLength();
				nearestCoinIndex = i;
			}
		}
		setTargetID(coinUnits[nearestCoinIndex]->getID());
		setTargetLoc(coinUnits[nearestCoinIndex]->getPositionComponent()->getPosition());
	}

	if (foundNewDestination == false)
	{
		mTargetID = pOwner->getID();
		mTargetLoc = pOwner->getPositionComponent()->getPosition();
	}

	//updatePath();
}