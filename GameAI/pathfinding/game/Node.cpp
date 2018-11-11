#include "Node.h"
#include "Grid.h"
#include "GameApp.h"


Node::Node()
:mId(BAD_NODE_ID),mCost(0),mDirection(0)
{
}

Node::Node( const NODE_ID& id )
:mId(id), mCost(0), mDirection(0)
{
}

Node::~Node()
{
}