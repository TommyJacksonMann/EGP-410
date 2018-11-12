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

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	mpGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	mpGridGraph = pGame->getGridGraph();

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
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	
	int currentNodeIndex = mpGrid->getSquareIndexFromPixelXY((int)pOwner->getPositionComponent()->getPosition().getX(), (int)pOwner->getPositionComponent()->getPosition().getY());
	mpCurrentNode = mpGridGraph->getNode(currentNodeIndex);
	Vector2D currentMovementDirection = mpCurrentNode->getDirectionVector();
	if (mpCurrentNode->getCost() == 0)
	{
		data.vel = Vector2D(0,0);
	}
	else
	{
		
		data.vel = currentMovementDirection * data.maxSpeed;
	}
	

	float velocityDirection = atan2(currentMovementDirection.getY(), currentMovementDirection.getX()) + .5f*3.14;
	pOwner->getPositionComponent()->setFacing(velocityDirection);

	this->mData = data;
	return this;
}
