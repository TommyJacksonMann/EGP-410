#include <cassert>
#include "ComponentManager.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"


ComponentID ComponentManager::msNextPositionComponentID = 0;
ComponentID ComponentManager::msNextPhysicsComponentID = 0;
ComponentID ComponentManager::msNextSteeringComponentID = 0;
ComponentID ComponentManager::msNextCollisionComponentID = 0;

using namespace std;

ComponentManager::ComponentManager(Uint32 maxSize)
	:mPositionPool(maxSize, sizeof(PositionComponent))
	, mPhysicsPool(maxSize, sizeof(PhysicsComponent))
	, mSteeringPool(maxSize, sizeof(SteeringComponent))
	, mCollisionPool(maxSize, sizeof(CollisionComponent))
{
}

ComponentManager::~ComponentManager()
{
	clear();
}


void ComponentManager::clear()
{
	//call destructor for all components
	for (auto& it : mPositionComponentMap)
	{
		PositionComponent* pComponent = it.second;
		pComponent->~PositionComponent();
	}
	for (auto& it : mPhysicsComponentMap)
	{
		PhysicsComponent* pComponent = it.second;
		pComponent->~PhysicsComponent();
	}
	for (auto& it : mSteeringComponentMap)
	{
		SteeringComponent* pComponent = it.second;
		pComponent->~SteeringComponent();
	}
	for (auto& it : mCollisionComponentMap)
	{
		CollisionComponent* pComponent = it.second;
		pComponent->~CollisionComponent();
	}

	//clear maps
	mPositionComponentMap.clear();
	mPhysicsComponentMap.clear();
	mSteeringComponentMap.clear();
	mCollisionComponentMap.clear();

	//reset memory pools
	mPositionPool.reset();
	mPhysicsPool.reset();
	mSteeringPool.reset();
	mCollisionPool.reset();
}


PositionComponent* ComponentManager::getPositionComponent(const ComponentID& id)
{
	auto it = mPositionComponentMap.find(id);
	if (it != mPositionComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

ComponentID ComponentManager::allocatePositionComponent(const PositionData& data, bool shouldWrap)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mPositionPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextPositionComponentID;
		PositionComponent* pComponent = new (ptr)PositionComponent(newID,shouldWrap);
		pComponent->setData(data);
		mPositionComponentMap[newID] = pComponent;
		msNextPositionComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocatePositionComponent(const ComponentID& id)
{
	auto it = mPositionComponentMap.find(id);

	if (it != mPositionComponentMap.end())//found it
	{
		PositionComponent* ptr = it->second;
		mPositionComponentMap.erase(it);

		ptr->~PositionComponent();
		mPositionPool.freeObject((Byte*)ptr);
	}
}

PhysicsComponent* ComponentManager::getPhysicsComponent(const ComponentID& id)
{
	auto it = mPhysicsComponentMap.find(id);
	if (it != mPhysicsComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

ComponentID ComponentManager::allocatePhysicsComponent(const ComponentID& positionComponentID, const PhysicsData& data)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mPhysicsPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextPhysicsComponentID;
		PhysicsComponent* pComponent = new (ptr)PhysicsComponent(newID, positionComponentID);
		pComponent->setData(data);
		pComponent->setPositionComponent(getPositionComponent(positionComponentID));
		mPhysicsComponentMap[newID] = pComponent;
		msNextPhysicsComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocatePhysicsComponent(const ComponentID& id)
{
	auto it = mPhysicsComponentMap.find(id);

	if (it != mPhysicsComponentMap.end())//found it
	{
		PhysicsComponent* ptr = it->second;
		mPhysicsComponentMap.erase(it);

		//hold for later
		Byte* pByte = (Byte*)ptr;

		ptr->~PhysicsComponent();
		mPhysicsPool.freeObject(pByte);
	}
}

SteeringComponent* ComponentManager::getSteeringComponent(const ComponentID& id)
{
	auto it = mSteeringComponentMap.find(id);
	if (it != mSteeringComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

ComponentID ComponentManager::allocateSteeringComponent(const ComponentID& physicsComponentID, const SteeringData& data)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mSteeringPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextSteeringComponentID;
		SteeringComponent* pComponent = new (ptr)SteeringComponent(newID, physicsComponentID);
		pComponent->setData(data);
		pComponent->setPhysicsComponent(getPhysicsComponent(physicsComponentID));
		mSteeringComponentMap[newID] = pComponent;
		msNextSteeringComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocateSteeringComponent(const ComponentID& id)
{
	auto it = mSteeringComponentMap.find(id);

	if (it != mSteeringComponentMap.end())//found it
	{
		SteeringComponent* ptr = it->second;
		mSteeringComponentMap.erase(it);

		//hold for later
		Byte* pByte = (Byte*)ptr;

		ptr->~SteeringComponent();
		mSteeringPool.freeObject(pByte);
	}
}

CollisionComponent* ComponentManager::getCollisionComponent(const ComponentID& id)
{
	auto it = mCollisionComponentMap.find(id);
	if (it != mCollisionComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

ComponentID ComponentManager::allocateCollisionComponent(const ComponentID& positionComponentID, const CollisionData& data)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mCollisionPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextCollisionComponentID;
		CollisionComponent* pComponent = new (ptr)CollisionComponent(newID, positionComponentID);
		pComponent->setData(data);
		pComponent->setPositionComponent(getPositionComponent(positionComponentID));
		mCollisionComponentMap[newID] = pComponent;
		msNextCollisionComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocateCollisionComponent(const ComponentID& id)
{
	auto it = mCollisionComponentMap.find(id);

	if (it != mCollisionComponentMap.end())//found it
	{
		CollisionComponent* ptr = it->second;
		mCollisionComponentMap.erase(it);

		//hold for later
		Byte* pByte = (Byte*)ptr;

		ptr->~CollisionComponent();
		mCollisionPool.freeObject(pByte);
	}
}

const float ELAPSED_TIME_PHYSICS_MULT = 1.0f;

void ComponentManager::update(float elapsedTime)
{
	updateSteering(elapsedTime*ELAPSED_TIME_PHYSICS_MULT);
	updatePhysics(elapsedTime*ELAPSED_TIME_PHYSICS_MULT);
	updateCollision(elapsedTime*ELAPSED_TIME_PHYSICS_MULT);

}

void ComponentManager::updatePhysics(float elapsedTime)
{
	for (auto& it : mPhysicsComponentMap)
	{
		PhysicsComponent* pPhysics = it.second;
		assert(pPhysics != NULL);
		PositionComponent* pPos = pPhysics->mpPositionComponent;
		assert( pPos != NULL);
		pPhysics->integrate(*pPos, elapsedTime);
	}
}

void ComponentManager::updateSteering(float elapsedTime)
{
	for (auto& it : mSteeringComponentMap)
	{
		SteeringComponent* pSteering = it.second;
		assert(pSteering != NULL);
		PhysicsComponent* pPhysics = pSteering->mpPhysicsComponent;
		assert(pPhysics != NULL);
		pSteering->applySteering(*pPhysics);
	}

}

//Compares collidors with every other collision (not as slow as I thought)
void ComponentManager::updateCollision(float elapsedTime)
{
	for (auto& it : mCollisionComponentMap)
	{
		CollisionComponent* pCollision = it.second;
		assert(pCollision != NULL);
		pCollision->updateDataPosition();
		ComponentID firstID = pCollision->getID();
		for (auto& itSecond : mCollisionComponentMap)
		{
			CollisionComponent* pSecondCollision = itSecond.second;
			assert(pSecondCollision != NULL);
			ComponentID secondID = pSecondCollision->getID();
			
			//if they aren't the same then update
			if (secondID != firstID)
			{
				pCollision->update(pSecondCollision);
				pSecondCollision = NULL;
			}
		}
	}
}