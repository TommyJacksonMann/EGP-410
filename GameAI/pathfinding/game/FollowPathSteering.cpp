#include "FollowPathSteering.h"
#include "Path.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"



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
Steering* FollowPathSteering::getSteering()
{
	/*Get CurrentDestination from Vector of Nodes
	* when in targetDistance of next node increment Node location to next node
	* if Node Location is greater than size, return NULL
	*Issues:
	* where should path be stored ~ Fixed (is private variable)
	* how can there be more than one path in main gridPathfinder Variable 
	* Old pathfinding rot vel needs to be set to 0 when not doing anything ~ Fixed
	* Sometimes the path is too big and looks for position outside screen ( A* Heuristic acts funny because the node doesn't exist on screen)
	* One Path is being visualized; May need to fix how this works
	*/
	if (!mpPath)
	{
		return this;
	}
	if (mCurrentPathPosition > mpPath->getNumNodes() || !mpPath->peekNode(mCurrentPathPosition))
	{
		return this;
	}

	//Get the position of the next node to move to
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D currentTargetNodePosition = pGrid->getULCornerOfSquare(mpPath->peekNode(mCurrentPathPosition)->getId());	//Watch this when the unit goes to a position off the screen

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	//Get the difference of the node position and the unit position
	Vector2D diff = currentTargetNodePosition - pOwner->getPositionComponent()->getPosition();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float targetSpeed;
	Vector2D targetVelocity;

	if (diff.getLength() < mTargetRadius)
	{
		//Stutters at every node
		data.vel = ZERO_VECTOR2D;
		data.acc = ZERO_VECTOR2D;
		data.rotAcc = 0;
		data.rotVel = 0;


		mCurrentPathPosition++;
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