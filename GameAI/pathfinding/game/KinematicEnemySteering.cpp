#include "KinematicEnemySteering.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Grid.h"
#include "GridGraph.h"
#include <cassert>


KinematicEnemySteering::KinematicEnemySteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::KINEMATIC_ENEMY_ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mMovementFactor = pGame->getEnemySpeed();
	mChangeDirectionFrequency = pGame->getEnemyDirectionFrquency();
}

Steering* KinematicEnemySteering::getSteering()
{
	Vector2D targetDirection;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}


	targetDirection = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (targetDirection.getLength() < mMovementFactor)
	{
		pOwner->getPositionComponent()->setPosition(mTargetLoc);

		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		Node* pToNode;
		Node* pTestWall;
		Vector2D newDestination;
		Vector2D newDirection;

		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();

		//ADD A WAY TO NOT GO BACKWARDS
		do {
			pToNode = NULL;
			newDestination = pOwner->getPositionComponent()->getPosition();
			//newDestination += Vector2D(16, 16);
			newDirection = Vector2D(0, 0);
			newDestination += newDirection;

			int randomDir = -1;

			/* 0-3 cardinal directions
			* 0 = up
			* 1 = right
			* 2 = down
			* 3 = left
			*/
			Vector2D testDirection;

			switch (mLastDirection)
			{
			case 0:			//UP
			{
				testDirection += Vector2D(0, -32);
				break;
			}
			case 1:			//RIGHT
			{
				testDirection += Vector2D(32, 0);
				break;
			}
			case 2: 		//DOWN
			{
				testDirection += Vector2D(0, 32);
				break;
			}
			case 3: 	//LEFT
			{
				testDirection += Vector2D(-32, 0);
				break;
			}
			default: {}
			}

			int testIfContinueIsWall = pGrid->getSquareIndexFromPixelXY(newDestination.getX() + testDirection.getX()
				, newDestination.getY() + testDirection.getY());

			pTestWall = pGridGraph->getNode(testIfContinueIsWall);

			float currentTime = pGame->getCurrentTime();
			float testTime = mLastChangedDirection + mChangeDirectionFrequency;


			if (pTestWall->getIsWall())
			{
				do
				{
  					randomDir = rand() % 4;
				} while (randomDir == mLastDirection);
			}
			else if (/*check frequency*/ testTime <= currentTime)
			{
				randomDir = rand() % 4;
				mLastChangedDirection = pGame->getCurrentTime();
			}
			else
			{
  				newDestination = Vector2D(newDestination.getX() + testDirection.getX()
					, newDestination.getY() + testDirection.getY());
				break;
			}

			switch (randomDir)
			{
			case 0:			//UP
			{
				newDirection += Vector2D(0, -32);
				mLastDirection = 0;
				break;
			}
			case 1:			//RIGHT
			{
				newDirection += Vector2D(32, 0);
				mLastDirection = 1;
				break;
			}
			case 2: 		//DOWN
			{
				newDirection += Vector2D(0, 32);
				mLastDirection = 2;
				break;
			}
			case 3: 	//LEFT
			{
				newDirection += Vector2D(-32, 0);
				mLastDirection = 3;
				break;
			}
			default: {assert(true); } //how would this ever happen
			}


			int toIndex = pGrid->getSquareIndexFromPixelXY(newDestination.getX() + newDirection.getX()
				, newDestination.getY() + newDirection.getY());

			pToNode = pGridGraph->getNode(toIndex);
			newDestination = Vector2D(newDestination.getX() + newDirection.getX()
				, newDestination.getY() + newDirection.getY());


		} while (pToNode->getIsWall());


		setTargetLoc(newDestination);

	}
	else
	{
		float targetSpeed;
		Vector2D targetVelocity;
		targetDirection.normalize();

		targetVelocity = targetDirection;
		targetVelocity *= mMovementFactor;
		pOwner->getPositionComponent()->setPosition(pOwner->getPositionComponent()->getPosition() + targetVelocity);
		float velocityDirection = atan2(targetDirection.getY(), targetDirection.getX()) + .5f*3.14;
		pOwner->getPositionComponent()->setFacing(velocityDirection);
	}

	//data.vel = targetVelocity;



	this->mData = data;
	return this;
}