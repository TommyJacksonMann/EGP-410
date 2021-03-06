#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "ArriveSteering.h"
#include "../KinematicArriveSteering.h"
#include "../FollowPathSteering.h"
#include "../FlowFieldSteering.h"


SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
		case Steering::ARRIVE:
		{
			delete mpSteering;
			mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, data.targetID, false);
			break;
		}
		case Steering::KINEMATICARRIVE:
		{
			delete mpSteering;
			mpSteering = new KinematicArriveSteering(data.ownerID, data.targetLoc, data.targetID, false);
			break;
		}
		case Steering::FOLLOW_PATH:
		{
			delete mpSteering;
			mpSteering = new FollowPathSteering(data.ownerID);
			break;
		}
		case Steering::FLOW_FIELD:
		{
			delete mpSteering;
			mpSteering = new FlowFieldSteering(data.ownerID);
			break;
		}
		default:
		{

		}
	};
}

