#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "../GameApp.h"
#include "../GridGraph.h"
#include "../../common/Grid.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const CollisionData& collisionData /*= ZERO_COLLISION_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID idC = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = idC;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(idC);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);
		pUnit->mCollisionComponentID = pComponentManager->allocateCollisionComponent(pUnit->mPositionComponentID);
		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const CollisionData& collisionData /*= ZERO_COLLISION_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, collisionData , PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	GameApp* pGame = static_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	while (pGrid->getValueAtIndex(pGrid->getSquareIndexFromPixelXY(posX, posY)) == BLOCKING_VALUE)
	{
		posX = rand() % gpGame->getGraphicsSystem()->getWidth();
		posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	}
	Unit* pUnit = createUnit(sprite, false, PositionData(Vector2D(posX,posY),0), PhysicsData(Vector2D(velX,velY),Vector2D(0.1f,0.1f), 0.1f, 0.05f));
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::SEEK, Vector2D(gpGame->getGraphicsSystem()->getWidth()/2, gpGame->getGraphicsSystem()->getHeight()/2));
	}
	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);
		pComponentManager->deallocateCollisionComponent(pUnit->mCollisionComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				if (it->first != PLAYER_UNIT_ID)
				{
					deleteUnit(it->first);
				}
				
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

std::vector<Unit*> UnitManager::getUnitsWithinRadius(Vector2D center, float radius, Steering::SteeringType typeToLookFor)
{
	std::vector<Unit*> unitsWithinRadius;

	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		if (it->second->getSteeringComponent()->getType() == typeToLookFor)
		{
			Vector2D diff;
			diff = it->second->getPositionComponent()->getPosition() - center;
			if (!(diff.getX() > radius || diff.getY() > radius))
			{
				if (abs(diff.getLength()) < radius)
				{
					unitsWithinRadius.push_back(it->second);
				}
			}
		}
	}
	return unitsWithinRadius;
}

std::vector<Unit*> UnitManager::getUnitsOfType(Steering::SteeringType typeToLookFor)
{
	std::vector<Unit*> unitsOfType;

	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		
		if (it->second->getSteeringComponent()->getType() == typeToLookFor)
		{
			unitsOfType.push_back(it->second);
		}
	}
	return unitsOfType;
}
