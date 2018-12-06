#include "CollisionComponent.h"
#include "./SteeringFiles/PositionComponent.h"
#include "Game.h"
#include "GameApp.h"
#include "HandleCollisionMessage.h"
#include "GameMessageManager.h"
#include "./SteeringFiles/UnitManager.h"

//Compares a position to see if it is in the colliders position
bool CollisionComponent::checkCollision(const CollisionData otherColliderData)
{
	switch (mData.colType)
	{
	case CollisionType::CIRCLE:
	{
		if (otherColliderData.colType == CIRCLE)
		{
			float distSq = ((mData.pos.getX() - otherColliderData.pos.getX())
				* (mData.pos.getX() - otherColliderData.pos.getX()))
				+ ((mData.pos.getY() - otherColliderData.pos.getY())
					* (mData.pos.getY() - otherColliderData.pos.getY()));
			float radiusSq = (mData.xDistance + otherColliderData.xDistance)
				* (mData.xDistance + otherColliderData.xDistance);

			if (distSq <= radiusSq)
			{
				return true;
			}
		}

		else if (otherColliderData.colType == RECTANGLE)
		{
			Vector2D circleDistance = Vector2D(abs(mData.pos.getX() - otherColliderData.pos.getX()), abs(mData.pos.getY() - otherColliderData.pos.getY()));

			if (circleDistance.getX() > (otherColliderData.xDistance + mData.xDistance))
			{
				return false;
			}
			if (circleDistance.getY() > (otherColliderData.yDistance + mData.xDistance))
			{
				return false;
			}

			if (circleDistance.getX() <= (otherColliderData.xDistance))
			{
				return true;
			}
			if (circleDistance.getY() <= (otherColliderData.yDistance))
			{
				return true;
			}

			float cornerDistance = ((circleDistance.getX() - otherColliderData.xDistance)
				* (circleDistance.getX() - otherColliderData.xDistance))
				+ ((circleDistance.getY() - otherColliderData.yDistance)
					* (circleDistance.getY() - otherColliderData.yDistance));
			return (cornerDistance <= (mData.xDistance * mData.xDistance));
		}
		break;
	}
	case CollisionType::RECTANGLE:
	{
		if (otherColliderData.colType == RECTANGLE)
		{
			Vector2D topLeftThis = Vector2D(mData.pos.getX() - mData.xDistance,
				mData.pos.getY() - mData.yDistance / 2);
			Vector2D bottomRightThis = Vector2D(mData.pos.getX() + mData.xDistance,
				mData.pos.getY() + mData.yDistance / 2);

			Vector2D topLeftOther = Vector2D(otherColliderData.pos.getX() - otherColliderData.xDistance,
				otherColliderData.pos.getY() - otherColliderData.yDistance / 2);
			Vector2D bottomRightOther = Vector2D(otherColliderData.pos.getX() + otherColliderData.xDistance,
				otherColliderData.pos.getY() + otherColliderData.yDistance / 2);

			if (topLeftThis.getX() > bottomRightOther.getX() || topLeftOther.getX() > bottomRightThis.getX())
			{
				return false;
			}

			if (topLeftThis.getY() < bottomRightOther.getY() || topLeftOther.getY() < bottomRightThis.getY())
			{
				return false;
			}

			return true;
		}
		else if (otherColliderData.colType == CIRCLE)
		{
			Vector2D circleDistance = Vector2D(abs(otherColliderData.pos.getX() - mData.pos.getX()), abs(otherColliderData.pos.getY() - mData.pos.getY()));

			if (circleDistance.getX() > (mData.xDistance + otherColliderData.xDistance))
			{
				return false;
			}
			if (circleDistance.getY() > (mData.yDistance + otherColliderData.xDistance))
			{
				return false;
			}

			if (circleDistance.getX() <= (mData.xDistance))
			{
				return true;
			}
			if (circleDistance.getY() <= (mData.yDistance))
			{
				return true;
			}

			float cornerDistance = ((circleDistance.getX() - mData.xDistance)
				* (circleDistance.getX() - mData.xDistance))
				+ ((circleDistance.getY() - mData.yDistance)
					* (circleDistance.getY() - mData.yDistance));
			return (cornerDistance <= (otherColliderData.xDistance * otherColliderData.xDistance));
		}

		break;
	}
	case ERR:
	default:
		//Collision Turned OFF
		return false;
	}
	return false;
}

void CollisionComponent::updateDataPosition()
{
	mData.pos = mpPositionComponent->getPosition();
}

void CollisionComponent::update(CollisionComponent* pComponent)
{
	bool isColliding = checkCollision(pComponent->getData());
		
	

	if (!mJustCollided && isColliding)
	{

		Unit* firstUnit = NULL;
		Unit* secondUnit = NULL;
		ComponentID thisID = getID();
		ComponentID paramID = pComponent->getID();

		std::map < UnitID, Unit*> tempMap = gpGame->getUnitManager()->getUnitMap();
		//Loop finds which unit has the coresponding Component
		std::map<UnitID, Unit*>::iterator iter = tempMap.begin();
		for (iter; iter != tempMap.end(); iter++)
		{
			if (iter->second->compareCollisionComponentID(paramID) && !secondUnit)
			{				
				//Since it is in a for loop if the object has already collided with this object ignore collision and don't fire a collision message
				secondUnit = iter->second;
				UnitID newCollide = secondUnit->getID();
				
				if (newCollide == mLastCollidedID || thisID == secondUnit->getCollisionComponent()->getLastCollidedID())
				{
					return;
				}
				mLastCollidedID = newCollide;
				if (firstUnit) 
				{
					break;
				}
			}

			if (iter->second->compareCollisionComponentID(thisID) && !firstUnit)
			{
				firstUnit = iter->second;
				if (secondUnit)
				{
					break;
				}
			}
		}
			
		//handle Collision Message
  		GameMessage* pMessage = new HandleCollisionMessage(firstUnit, secondUnit);
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->getMessageManager()->addMessage(pMessage, 0);
		mJustCollided = true;
	}
	else if(mJustCollided && isColliding)
	{
		mJustCollided = true;
	}
	else
	{
		mJustCollided = false;
		
	}	
}

//Used for reseting movement
void CollisionComponent::resetLastCollided()
{
	mLastCollidedID = INVALID_ID;
	mJustCollided = false;
}