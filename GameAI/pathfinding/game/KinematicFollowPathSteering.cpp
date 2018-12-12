#include "KinematicFollowPathSteering.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Grid.h"
#include "GridGraph.h"
#include <cassert>
#include "PathPool.h"
#include "GridPathfinder.h"


KinematicFollowPathSteering::KinematicFollowPathSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::KINEMATIC_ENEMY_CHASE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mMovementFactor = pGame->getEnemySpeed();
	updatePath();
}

void KinematicFollowPathSteering::updatePath()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
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


Steering* KinematicFollowPathSteering::getSteering()
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
			updatePath();
		}
		else
		{
			mCurrentPathPosition++;
			//setTargetLoc( pGrid->getULCornerOfSquare(mPath.peekNode(mCurrentPathPosition)->getId()) );
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