#include "AStarPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include "Vector2D.h"
#include "GameApp.h"
#include "Grid.h"

using namespace std;

AStarPathfinder::AStarPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

AStarPathfinder::~AStarPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* AStarPathfinder::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//allocate nodes to visit list and place starting node in it
	list<Node*> nodesToVisit;
	nodesToVisit.push_front(pFrom);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	//create Path
	Path* pPath = new Path();

	bool toNodeAdded = false;
	NodeRecord* startRecord = new NodeRecord(pFrom, NULL, 0, 0, Category::UNVISITED);
	startRecord->mEstimatedTotalCost = getHeuristic(pFrom, pTo);
	NodeRecord* current;
	vector<NodeRecord*> openNodes;
	openNodes.push_back(startRecord);
	pPath->addNode(pFrom);
	vector<NodeRecord*> closedNodes;

	while (openNodes.size() > 0)
	{
		current = getSmallestNodeRecord(openNodes);
		int currentNodeIndex = getSmallestNodeRecordIndex(openNodes);
		if (current->mpNode == pTo)
		{
			toNodeAdded = true;
			break;
		}

		vector<Connection*> connections = mpGraph->getConnections(current->mpNode->getId());
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			NodeRecord* endNodeRecord;
			Node* endNode = connections[i]->getToNode();
			float endNodeCost = current->mCostSoFar + connections[i]->getCost() + getHeuristic(endNode, pTo);;
			float endNodeHeuristic = getHeuristic(endNode, pTo);
			bool closedContainsEndNode = false;
			bool openContainsEndNode = false;
			bool visitedNodesContainsEndNode = false;
			
			unsigned int closedNodeIndex;
			for (closedNodeIndex = 0; closedNodeIndex < closedNodes.size(); closedNodeIndex++)
			{
				if (closedNodes[closedNodeIndex]->mpNode == endNode)
				{
					closedContainsEndNode = true;
					break;
				}
			}
			vector<Node*>::iterator visitedMemberLocation;
			for (visitedMemberLocation = mVisitedNodes.begin(); visitedMemberLocation != mVisitedNodes.end(); ++visitedMemberLocation)
			{
				if ((*visitedMemberLocation) == endNode)
				{
					visitedNodesContainsEndNode = true;
					break;
				}
			}
			unsigned int openNodeIndex;
			for (openNodeIndex = 0; openNodeIndex < openNodes.size(); openNodeIndex++)
			{
				if (openNodes[openNodeIndex]->mpNode == endNode)
				{
					openContainsEndNode = true;
					break;
				}
			}
			if (closedContainsEndNode) 
			{
				endNodeRecord = closedNodes[closedNodeIndex];
				if (endNodeRecord->mCostSoFar <= endNodeCost)
				{
					continue;
				}
				//mVisitedNodes.erase(visitedMemberLocation);
				closedNodes.erase(closedNodes.begin()+closedNodeIndex);
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}
			else if (openContainsEndNode)
			{
				endNodeRecord = openNodes[openNodeIndex];
				if (endNodeRecord->mCostSoFar <= endNodeCost)
				{
					continue;
				}
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->mpNode = endNode;
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}
			endNodeRecord->mCostSoFar = endNodeCost;
			endNodeRecord->mpConnection = connections[i];
			endNodeRecord->mEstimatedTotalCost = endNodeCost + endNodeHeuristic;
			if (!openContainsEndNode && !toNodeAdded)
			{
				openNodes.push_back(endNodeRecord);
				if (endNodeRecord->mpNode == pTo)
				{
					toNodeAdded = true;
				}
				//Also add it to the closed list
#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(endNodeRecord->mpNode);
#endif
				closedNodes.push_back(endNodeRecord);
				pPath->addNode(endNodeRecord->mpNode);
			}
		}

		openNodes.erase(openNodes.begin() + currentNodeIndex);
		closedNodes.push_back(current);
	}

	if (toNodeAdded == false)
	{
		return NULL;
	}
	else
	{
		while (current->mpNode != pFrom)
		{
			pPath->addNode(current->mpNode);
			current->mpNode = current->mpConnection->getFromNode();
			for (int i = 0; i < closedNodes.size(); i++)
			{
				if (closedNodes[i]->mpNode == current->mpNode)
				{
					current->mpConnection = closedNodes[i]->mpConnection;
					break;
				}
			}
		}
		pPath->addNode(current->mpNode);
	}

	
	/*for (unsigned int i = 0; i < closedNodes.size(); i++)
	{
		if (closedNodes[i] != nullptr)
		{
			delete closedNodes[i];
		}
		
	}
	for (unsigned int i = 0; i < openNodes.size(); i++)
	{
		delete openNodes[i];
	}
*/

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif
	return pPath;

}

float AStarPathfinder::getHeuristic(Node* pFrom, Node* pTo)
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D diff;
	diff = pGrid->getULCornerOfSquare(pFrom->getId()) - pGrid->getULCornerOfSquare(pTo->getId());
	return diff.getLength();
}

NodeRecord* AStarPathfinder::getSmallestNodeRecord(std::vector<NodeRecord*> vpNodeRecords)
{
	float lowestCost = INFINITY;
	int lowestNodeRecordIndex = 0;
	for (int i = 0; i < vpNodeRecords.size(); i++)
	{
		if (vpNodeRecords[i]->mEstimatedTotalCost < lowestCost)
		{
			lowestCost = vpNodeRecords[i]->mEstimatedTotalCost;
			lowestNodeRecordIndex = i;
		}
	}
	return vpNodeRecords[lowestNodeRecordIndex];
}

int AStarPathfinder::getSmallestNodeRecordIndex(std::vector<NodeRecord*> vpNodeRecords)
{
	float lowestCost = INFINITY;
	int lowestNodeRecordIndex = 0;
	for (int i = 0; i < vpNodeRecords.size(); i++)
	{
		if (vpNodeRecords[i]->mEstimatedTotalCost < lowestCost)
		{
			lowestCost = vpNodeRecords[i]->mEstimatedTotalCost;
			lowestNodeRecordIndex = i;
		}
	}
	return lowestNodeRecordIndex;
}