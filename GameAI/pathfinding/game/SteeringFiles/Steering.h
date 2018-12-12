#pragma once

/*Steering - implementation of Steering class from "Artificial Intelligence for Games" - Millington and Funge
*/

#include <Vector2D.h>
#include <Trackable.h>
#include "defines.h"
#include "PhysicsComponent.h"

const float DEFAULT_TARGET_RADIUS = 10;
const float DEFAULT_SLOW_RADIUS = 100;
const float DEFAULT_TIME_TO_TARGET = .5;

const float DEFAULT_TARGET_ROTATION_RADIUS = .05;
const float DEFAULT_SLOW_ROTATION_RADIUS = .5;
const float DEFAULT_TIME_TO_TARGET_ROTATION = .1;

const float DEFAULT_WANDER_OFFSET = 150;
const float DEFAULT_WANDER_RADIUS = 30;
const float DEFAULT_WANDER_RATE = .5;

const float DEFAULT_NEIGHBORHOOD_RADIUS = 100;
const float DEFAULT_CHASE_RADIUS = 300;

class Steering: public Trackable
{
public:
	enum SteeringType
	{
		INVALID_TYPE = -1,
		SEEK,
		FLEE,
		ARRIVE,
		KINEMATICARRIVE,
		FACE,
		WANDER,
		WANDERANDCHASE,
		ARRIVEANDFACE,
		ALIGN,
		SEPARATION,
		COHESION,
		FLOCK,
		FOLLOW_PATH,
		FLOW_FIELD,
		KINEMATIC_ENEMY_ARRIVE,
		KINEMATIC_ENEMY_CHASE,
		KINEMATIC_ENEMY_RUN,
		KINEMATIC_PLAYER_AI
	};

	//constructors and destructors
	Steering( SteeringType type = INVALID_TYPE ):mType(type), mTargetLoc(ZERO_VECTOR2D), mTargetID(INVALID_UNIT_ID), mOwnerID(INVALID_UNIT_ID), mData(ZERO_PHYSICS_DATA) {};
	virtual ~Steering(){};

	//accessors
	void setTargetLoc(const Vector2D& loc) { mTargetLoc = loc; };
	const Vector2D& getTargetLoc() const { return mTargetLoc; };
	void setTargetID(const UnitID& id) { mTargetID = id; };
	const UnitID& getTargetID() const { return mTargetID; };
	void setOwnerID(const UnitID& id) { mOwnerID = id; };
	//void setData(const PhysicsData& data) { mData = data; };
	const PhysicsData& getData() const { return mData; };
	void update();
protected:
	SteeringType mType;
	Vector2D mTargetLoc;
	UnitID mTargetID;
	UnitID mOwnerID;
	PhysicsData mData;

	virtual Steering* getSteering() { return this; };

};

