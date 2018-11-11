#include "GridGraph.h"
#include "Connection.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include <vector>
#include "GridGraph.h"
#include "GameApp.h"
#include "..\common\Game.h"
#include "..\common\Color.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "SpriteManager.h"
#include "GraphicsBuffer.h"
#include <string>

GridGraph::GridGraph( Grid* pGrid )
:Graph()
,mpGrid(pGrid)
{
}

GridGraph::~GridGraph()
{
}

void GridGraph::init()
{
	if( mpGrid != NULL )
	{
		int numGridIndices = mpGrid->getGridWidth() * mpGrid->getGridHeight();

		//size the node vector
		mNodes.resize( numGridIndices, NULL );
		
		//go through all elements of the grid and make Nodes for them - also add them to Node vector
		for( int i=0; i<numGridIndices; i++ )
		{
			//create a Node for this location
			Node* pNode = new Node( i );
			//add to NodeVector
			mNodes[i] = pNode;
		}

		//go through all elements of the grid - get adjacencies and create connections for each
		for( int i=0; i<numGridIndices; i++ )
		{
			//get the Node for this location
			Node* pFromNode = mNodes[i];

			//get all the adjacencies
			std::vector<int> adjacencies = mpGrid->getAdjacentIndices( i );

			//create a vector of Connections
			std::vector<Connection*> connections;

			//go through all adjacent indices, get the nodes for them, create connections, and add to connections map
			for( unsigned int adjIndex = 0; adjIndex < adjacencies.size(); adjIndex++ )
			{
				//check for blocking terrain
				if( mpGrid->getValueAtIndex(adjacencies[adjIndex]) != BLOCKING_VALUE )
				{

					int right = pFromNode->getId() + 1;
					int left = pFromNode->getId() - 1;
					int up = pFromNode->getId() - mpGrid->getGridWidth();
					int down = pFromNode->getId() + mpGrid->getGridWidth();
					//int upLeft = pFromNode->getId() - mpGrid->getGridWidth() - 1;
					//int upRight = pFromNode->getId() - mpGrid->getGridWidth() + 1;
					

					Node* pToNode = mNodes[ adjacencies[adjIndex] ];//find to node
					Connection* pConnection;

					bool notDiagonal = pToNode->getId() == right || pToNode->getId() == left || 
						pToNode->getId() == up || pToNode->getId() == down;
					if (notDiagonal)
					{
						pConnection = new Connection(pFromNode, pToNode, 1.0f);//create a connection
					}
					else
					{
						pConnection = new Connection(pFromNode, pToNode, sqrt(2));//create a connection
					}
					//add connection to connection vectors
					mConnections.push_back( pConnection );
					connections.push_back( pConnection );
				}
			}

			//put temp connection vector into map
			mConnectionMap[i] = connections;
		}
	}
}

void GridGraph::VisualizeNodeCosts()
{

	int i = 0;
	Node* pCurrentNode = getNode(i);
	while (pCurrentNode != NULL)
	{
		Vector2D ulPos = mpGrid->getULCornerOfSquare(i);
		int nodeCost = pCurrentNode->getDirection();
		float nodeDirection = pCurrentNode->getCost();
		gpGame->getGraphicsSystem()->draw(*gpGame->getSpriteManager()->getSprite(NODE_DIRECTION_SPRITE_ID), ulPos.getX() + 12, ulPos.getY() + 5, nodeDirection);
		gpGame->getGraphicsSystem()->writeText(*gpGame->getFont(), ulPos.getX(), ulPos.getY(), std::to_string(nodeCost).c_str(), Color(0, 0, 0));
		i++;
		pCurrentNode = getNode(i);
		
	}
}