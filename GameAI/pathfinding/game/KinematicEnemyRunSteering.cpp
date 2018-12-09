#include "KinematicEnemyRunSteering.h"
#include "Game.h"
#include "GameApp.h"
#include "./SteeringFiles/Unit.h"
#include "./SteeringFiles/UnitManager.h"
#include "Grid.h"
#include "GridGraph.h"
#include <cassert>

KinematicEnemyRunSteering::KinematicEnemyRunSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{

	
	mType = Steering::KINEMATIC_ENEMY_RUN;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mMovementFactor = pGame->getEnemySpeed();
}

Steering* KinematicEnemyRunSteering::getSteering()
{
	Vector2D targetDirection;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetPlayerPos = pTarget->getPositionComponent()->getPosition();
	}


	targetDirection = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	Vector2D heuristicDirection =  pOwner->getPositionComponent()->getPosition() - mTargetPlayerPos;

	if (targetDirection.getLength() < mMovementFactor)
	{
		pOwner->getPositionComponent()->setPosition(mTargetLoc);

		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		Node* pToNode;
		Node* pTestWall;
		Vector2D newDestination;
		//Vector2D newDirection;


		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();

		do {
			/* 0-3 cardinal directions
			* 0 = up
			* 1 = right
			* 2 = down
			* 3 = left
			*/

			pToNode = NULL;

			newDestination = pOwner->getPositionComponent()->getPosition();
			//newDirection = Vector2D(0, 0);
			//newDestination += newDirection;
			heuristicDirection.normalize();

			Vector2D testDirection;

			int randDir = -1;


			int currentDirection = -1;
			int opDirection = -1;

			if (heuristicDirection.getX() <= -0.5 && heuristicDirection.getX() < 0)
			{
				//left
				testDirection = Vector2D(-32, 0);
				currentDirection = 3;
				opDirection = 1;
			}
			else if (heuristicDirection.getX() > 0.5&& heuristicDirection.getX() >= 0)
			{
				//right
				testDirection = Vector2D(32, 0);
				currentDirection = 1;
				opDirection = 3;
			}
			if ((heuristicDirection.getY() <= -0.75 && heuristicDirection.getY() < 0) /*&& (heuristicDirection.getX() <= 0.5 || heuristicDirection.getX() >= -0.5)*/)
			{
				//down
				testDirection = Vector2D(0, -32);
				currentDirection = 2;
				opDirection = 0;
			}
			else if ((heuristicDirection.getY() > 0.75 && heuristicDirection.getY() >= 0)/* && (heuristicDirection.getX() <= 0.5 || heuristicDirection.getX() >= -0.5)*/)
			{
				//up
				testDirection = Vector2D(0, 32);
 				currentDirection = 0;
				opDirection = 2;
			}


			int wallTestIndex = pGrid->getSquareIndexFromPixelXY(newDestination.getX() + testDirection.getX(),
				newDestination.getY() + testDirection.getY());

			pTestWall = pGridGraph->getNode(wallTestIndex);

			if (pTestWall->getIsWall())
			{
				int newDir = -1;
				do
				{
					
					newDestination = pOwner->getPositionComponent()->getPosition();
					testDirection = ZERO_VECTOR2D;
					do
					{
						randDir = rand() % 4;
					} while (randDir == currentDirection || randDir == opDirection);

					testDirection = ZERO_VECTOR2D;
					switch (randDir)
					{
					case 0:			//UP
					{
						testDirection = Vector2D(0, 32);
						newDir = 0;
						break;
					}
					case 1:			//RIGHT
					{
						testDirection = Vector2D(32, 0);
						newDir = 1;
						break;
					}
					case 2: 		//DOWN
					{
						testDirection = Vector2D(0, -32);
						newDir = 2;
						break;
					}
					case 3: 	//LEFT
					{
						testDirection = Vector2D(-32, 0);
						newDir = 3;
						break;
					}
					default: {assert(true); } //how would this ever happen
					}

					int wallTestIndex = pGrid->getSquareIndexFromPixelXY(newDestination.getX() + testDirection.getX(),
						newDestination.getY() + testDirection.getY());
					if (wallTestIndex < 0)
						continue;
					
					pTestWall = pGridGraph->getNode(wallTestIndex);
					if(!pTestWall)
						continue;

					newDestination = Vector2D(newDestination.getX() + testDirection.getX(),
						newDestination.getY() + testDirection.getY());

				} while (pTestWall->getIsWall());

				mLastDirection = newDir;
			}
			else
			{
				newDestination = Vector2D(newDestination.getX() + testDirection.getX()
					, newDestination.getY() + testDirection.getY());
				mLastDirection = currentDirection;
				break;

			}
			int toIndex = pGrid->getSquareIndexFromPixelXY(newDestination.getX(), newDestination.getY());

			pToNode = pGridGraph->getNode(toIndex);

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
