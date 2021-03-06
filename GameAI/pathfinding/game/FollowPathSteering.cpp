#include "FollowPathSteering.h"
#include "Path.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GraphicsSystem.h"



FollowPathSteering::FollowPathSteering(const UnitID& ownerID)
	:Steering()
{
	setOwnerID(ownerID);


	mTargetRadius = DEFAULT_TARGET_RADIUS;
	mSlowRadius = DEFAULT_SLOW_RADIUS;
	mTimeToTarget = DEFAULT_TIME_TO_TARGET;
}


FollowPathSteering::~FollowPathSteering()
{
}

void FollowPathSteering::SetPath(Path* pPath)
{
	mPath = *pPath; mCurrentPathPosition = 1;
}

Steering* FollowPathSteering::getSteering()
{
	/*Get CurrentDestination from Vector of Nodes
	* when in targetDistance of next node increment Node location to next node
	* if Node Location is greater than size, return NULL
	*/
	
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	pGame->getPathfinder()->drawOtherPath(pGame->getGrid(), pGame->getGraphicsSystem()->getBackBuffer(), mPath);

	if (mCurrentPathPosition > mPath.getNumNodes() || !mPath.peekNode(mCurrentPathPosition))
	{
		return this;
	}

	//Get the position of the next node to move to
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D currentTargetNodePosition = pGrid->getULCornerOfSquare(mPath.peekNode(mCurrentPathPosition)->getId());	

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	//Get the difference of the node position and the unit position
	Vector2D diff = currentTargetNodePosition - pOwner->getPositionComponent()->getPosition();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float targetSpeed;
	Vector2D targetVelocity;

	if (diff.getLength() < mTargetRadius)
	{
		//Stutters at every node
		if (mCurrentPathPosition == mPath.getNumNodes() - 1)
		{
			data.vel = ZERO_VECTOR2D;
			data.acc = ZERO_VECTOR2D;
			data.rotAcc = 0;
			data.rotVel = 0;
		}
		else
		{
			mCurrentPathPosition++;
		}
		this->mData = data;
		return this;
	}

	if (diff.getLength() > mSlowRadius)
	{
		targetSpeed = data.maxSpeed;


	}
	else
	{
		targetSpeed = data.maxSpeed * diff.getLength() / mSlowRadius;
	}

	targetVelocity = diff;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	data.acc = targetVelocity - data.vel;
	data.acc /= mTimeToTarget;

	if (data.acc.getLength() > data.maxAccMagnitude)
	{
		data.acc.normalize();

		data.acc *= data.maxAccMagnitude;

	}


	float velocityDirection = atan2(diff.getY(), diff.getX()) + .5f*3.14;
	pOwner->getPositionComponent()->setFacing(velocityDirection);

	this->mData = data;
	return this;
}
