#pragma once
#include "./SteeringFiles/Component.h"
#include <Vector2D.h>

/*
* Class Implemented By RJ Bourdelais
* Implemented in Unit and Component Manager as well
*
*/



enum CollisionType
{
	CIRCLE,
	RECTANGLE,
	ERR
};

class PositionComponent;

struct CollisionData : public Trackable
{
	CollisionData() :colType(ERR), xDistance(0), yDistance(0) {};
	CollisionData(CollisionType theType, float theColSize, float theColY) :colType(theType), xDistance(theColSize), yDistance(theColY){};
	
	Vector2D pos;
	float xDistance;
	float yDistance;
	CollisionType colType;

	
};

const CollisionData ZERO_COLLISION_DATA;

class CollisionComponent : public Component
{
public:
	

	const CollisionType getCollisionType() const { return mData.colType; }
	void setCollisionType(const CollisionType type) { mData.colType = type; }
	const CollisionData& getData() const { return mData; }
	void setData(const CollisionData& data) { mData = data; }
	void setCollisionXSize(const float xSize) { mData.xDistance = xSize; }
	void setCollisionYSize(const float ySize) { mData.yDistance = ySize; }
	const float getCollisionXSize() const { return mData.xDistance; }
	const float getCollisionYSize() const { return mData.yDistance; }
	//const bool getIsColliding() const { return mIsColliding;  }

	const bool didItJustCollide() const { return mJustCollided; }
	bool checkCollision(const CollisionData otherColliderData);
	void updateDataPosition();
	void update(CollisionComponent* pComponent);
	typedef Uint32 UnitID;
	const UnitID getLastCollidedID() const { return mLastCollidedID; }

private:
	CollisionData mData;
	ComponentID mPositionComponentID;
	PositionComponent* mpPositionComponent = NULL;

	bool mJustCollided = false;
	
	UnitID mLastCollidedID;

	CollisionComponent(const ComponentID& id, const ComponentID& posID) : Component(id), mPositionComponentID(posID),mJustCollided(false) {};
	~CollisionComponent() {};

	void setPositionComponent(PositionComponent* pComp) { mpPositionComponent = pComp; };

	friend class ComponentManager;
};