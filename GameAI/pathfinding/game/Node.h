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
	int getCost() { return mCost; }

	void setDirection(float dir) { mDirection = dir; }
	void setCost(int cost) { mCost = cost; }
private:
	const NODE_ID mId;
	
	float mDirection;
	int mCost = 0;
};