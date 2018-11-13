#include "Node.h"
#include "Grid.h"
#include "GameApp.h"
#include "Game.h"
#include "Connection.h"
#include "Graph.h"
#include "GridGraph.h"


Node::Node()
:mId(BAD_NODE_ID),mCost(WALL_COST),mDirection(0), mDirectionVector(Vector2D(0,0))
{
}

Node::Node( const NODE_ID& id )
:mId(id), mCost(WALL_COST), mDirection(0), mDirectionVector(Vector2D(0, 0))
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
	Vector2D dir = pos1 - pos2;
	dir.normalize();
	mDirectionVector = dir;
	mDirection = atan2(dir.getY(), dir.getX());
}

bool Node::getIsWall()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridGraph* pGridGraph = pGame->getGridGraph();
	Graph* pGraph = pGridGraph;
	std::vector<Connection*> pConnections = pGraph->getConnections(getId());
	bool isWall = true;
	//GOES THROUGH ALL OF TONODE's CONNECTIONS
	for (int i = 0; i < pConnections.size(); i++)
	{
		Node* pCurrent = pConnections[i]->getToNode();
		std::vector<Connection*> pCurrentConnections = pGraph->getConnections(pCurrent->getId());

		//CHECKS TO SEE IF THE TONODE OF THE CONNECTION HAS A CONNECTION BACK TO THE TONODE
		for (int j = 0; j < pCurrentConnections.size() && isWall == true; j++)
		{
			if (pCurrentConnections[j]->getToNode() == this)
			{
				isWall = false;
			}
		}
		if (!isWall)
		{
			break;
		}
	}
	return isWall;
}