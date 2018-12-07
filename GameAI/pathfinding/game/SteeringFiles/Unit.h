#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "../CollisionComponent.h"
#include "Sprite.h"
#include "Steering.h"

//#include "CircularQueue.h"
//#include "Transaction.h"
//#include "TransactionProcessor.h"

class PhysicsComponent;
class SteeringComponent;
class CollisionComponent;
class Sprite;
class UnitManager;
class StateMachine;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;

enum UnitType
{
	PLAYER, 
	ENEMY,
	COIN,
	UNIT
};


//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	void draw() const;
	float getFacing() const;
	void update(float elapsedTime);

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	CollisionComponent* getCollisionComponent() const;
	StateMachine* getStateMachine() { return mpStateMachine; }
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	void setUnitType(const UnitType type) { mUnitType = type; }
	const UnitType getUnitType() const { return mUnitType; }
	const bool compareCollisionComponentID(ComponentID componentID);
	const UnitID getID() const { return mID; }

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);
	void setCollision(CollisionType type = CollisionType::ERR, const float xSize = 0, const float ySize = 0);

private:
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	ComponentID mCollisionComponentID;
	PositionComponent* mpPositionComponent = NULL;
	StateMachine* mpStateMachine;
	Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	UnitType mUnitType;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
