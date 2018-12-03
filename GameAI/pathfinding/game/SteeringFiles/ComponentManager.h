#include <map>
#include <Trackable.h>
#include <MemoryPool.h>
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "../CollisionComponent.h"

class ComponentManager : public Trackable
{
public:
	ComponentManager(Uint32 maxSize);
	~ComponentManager();

	void clear();

	PositionComponent* getPositionComponent(const ComponentID& id);
	ComponentID allocatePositionComponent(const PositionData& data = ZERO_POSITION_DATA, bool shouldWrap=true);
	void deallocatePositionComponent(const ComponentID& id);

	PhysicsComponent* getPhysicsComponent(const ComponentID& id);
	ComponentID allocatePhysicsComponent(const ComponentID& positionComponentID, const PhysicsData& data = ZERO_PHYSICS_DATA);
	void deallocatePhysicsComponent(const ComponentID& id);

	SteeringComponent* getSteeringComponent(const ComponentID& id);
	ComponentID allocateSteeringComponent(const ComponentID& physicsComponentID, const SteeringData& data = ZERO_STEERING_DATA);
	void deallocateSteeringComponent(const ComponentID& id);

	CollisionComponent* getCollisionComponent(const ComponentID& id);
	ComponentID allocateCollisionComponent(const ComponentID& positionComponentID, const CollisionData& data = ZERO_COLLISION_DATA);
	void deallocateCollisionComponent(const ComponentID& id);


	void update(float elapsedTime);

private:
	std::map<ComponentID, PositionComponent*> mPositionComponentMap;
	MemoryPool mPositionPool;
	std::map<ComponentID, PhysicsComponent*> mPhysicsComponentMap;
	MemoryPool mPhysicsPool;
	std::map<ComponentID, SteeringComponent*> mSteeringComponentMap;
	MemoryPool mSteeringPool;
	std::map<ComponentID, CollisionComponent*> mCollisionComponentMap;
	MemoryPool mCollisionPool;


	static ComponentID msNextPositionComponentID;
	static ComponentID msNextPhysicsComponentID;
	static ComponentID msNextSteeringComponentID;
	static ComponentID msNextCollisionComponentID;

	void updatePhysics(float elapsedTime);
	void updateSteering(float elapsedTime);
	void updateCollision(float elapsedTime);
};