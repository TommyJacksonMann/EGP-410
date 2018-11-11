#include "Node.h"
#include "Grid.h"
#include "GameApp.h"


Node::Node()
:mId(BAD_NODE_ID),mCost(0),mDirection(Vector2D(0,0))
{
}

Node::Node( const NODE_ID& id )
:mId(id), mCost(0), mDirection(Vector2D(0, 0))
{
}

Node::~Node()
{
}

void Node::setDirection(Node* pNode)
{
	Grid* tempGrid = static_cast<GameApp*>(gpGame)->getGrid();
	Vector2D pos1 = tempGrid->getULCornerOfSquare(pNode->getId());
	Vector2D pos2 = tempGrid->getULCornerOfSquare(mId);

	mDirection = (pos1 - pos2);
	mDirection.normalize();
}