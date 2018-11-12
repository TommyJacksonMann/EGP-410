#pragma once

#include <Trackable.h>
#include <Vector2D.h>

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};
	float getDirection() { return mDirection; }
	Vector2D getDirectionVector() { return mDirectionVector; }
	int getCost() { return mCost; }

	void setDirection(float dir) { mDirection = dir; }
	void setDirectionVector(Vector2D dir) { mDirectionVector = dir; }
	void setDirection(Node* pNode);
	void setCost(int cost) { mCost = cost; }
private:
	const NODE_ID mId;
	
	Vector2D mDirectionVector;
	float mDirection;
	int mCost = 0;
};