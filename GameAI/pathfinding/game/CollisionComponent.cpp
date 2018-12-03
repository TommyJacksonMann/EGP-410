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

		//COPY THIS TO THE OTHER SWITCH
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
	bool mIsColliding = checkCollision(pComponent->getData());


	if (mIsColliding)
	{

		Unit* firstUnit = NULL;
		Unit* secondUnit = NULL;

		std::map < UnitID, Unit*> tempMap = gpGame->getUnitManager()->getUnitMap();
		std::map<UnitID, Unit*>::iterator iter = tempMap.begin();
		for (iter; iter != tempMap.end(); iter++)
		{
			if (iter->second->compareComponentID(pComponent->getID()))
			{
				secondUnit = iter->second;
				if (firstUnit)
					break;
			}

			if (iter->second->compareComponentID(this->getID()))
			{
				firstUnit = iter->second;
				if (secondUnit)
					break;
			}
		}

		//handle Collision Message
		GameMessage* pMessage = new HandleCollisionMessage(firstUnit, secondUnit);
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		pGame->getMessageManager()->addMessage(pMessage, 0);
	}
}