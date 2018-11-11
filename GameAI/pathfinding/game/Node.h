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
	Vector2D getDirection() { return mDirection; }
	int getCost() { return mCost; }

	void setDirection(Node* pNode);
	void setCost(int cost) { mCost = cost; }
private:
	const NODE_ID mId;
	
	Vector2D mDirection;
	int mCost = 0;
};