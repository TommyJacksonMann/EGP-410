#include "FlowFieldSteering.h"
#include "Path.h"
#include "Node.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Game.h"
#include "GameApp.h"
#include "Connection.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GraphicsSystem.h"



FlowFieldSteering::FlowFieldSteering(const UnitID& ownerID)
	:Steering()
{
	setOwnerID(ownerID);


	mTargetRadius = DEFAULT_TARGET_RADIUS;
	mSlowRadius = DEFAULT_SLOW_RADIUS;
	mTimeToTarget = DEFAULT_TIME_TO_TARGET;
}


FlowFieldSteering::~FlowFieldSteering()
{
}

void FlowFieldSteering::SetNode(Node* pNode)
{
	mpCurrentNode = pNode; 
}

Steering* FlowFieldSteering::getSteering()
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

	GameApp* pGame = static_cast<GameApp*>(gpGame);

	if (mpCurrentNode->getCost() == 0 )
	{
		return this;
	}

	//Get the position of the next node to move to
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Vector2D currentTargetNodePosition = pGrid->getULCornerOfSquare(mpCurrentNode->getId());	//Watch this when the unit goes to a position off the screen

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	//Get the difference of the node position and the unit position
	Vector2D diff = currentTargetNodePosition - pOwner->getPositionComponent()->getPosition();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	float targetSpeed;
	Vector2D targetVelocity;

	if (diff.getLength() < mTargetRadius)
	{
		//Stutters at every node
		if (mpCurrentNode->getCost() == 0)
		{
			data.vel = ZERO_VECTOR2D;
			data.acc = ZERO_VECTOR2D;
			data.rotAcc = 0;
			data.rotVel = 0;
		}
		else
		{
			
			std::vector<Connection*> pConnections = pGridGraph->getConnections(mpCurrentNode->getId());
			int nodeCost = 1000000;
			int nodeIndex = 0;
			for (int i = 0; i < pConnections.size(); i++)
			{
				if (pConnections[i]->getToNode()->getCost() < nodeCost)
				{
					nodeCost = pConnections[i]->getToNode()->getCost();
					nodeIndex = i;
				}
			}
			mpCurrentNode = pConnections[nodeIndex]->getToNode();
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
