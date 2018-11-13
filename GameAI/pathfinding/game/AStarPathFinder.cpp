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
#include "PriorityQueue.h"

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

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	//mVisitedNodes.push_back(pFrom);
#endif

	//create Path
	Path* pPath = new Path();

	bool toNodeAdded = false;
	NodeRecord startRecord = NodeRecord(pFrom, NULL, 0, 0, Category::UNVISITED);
	startRecord.mEstimatedTotalCost = getHeuristic(pFrom, pTo);
	NodeRecord current;
	PriorityQueue<NodeRecord, vector<NodeRecord>, CompareEstimatedCost> openNodes, closedNodes;

	openNodes.push(startRecord);

	while (openNodes.size() > 0)
	{
		//gets the smallest available node and its index
		current = openNodes.top();
		openNodes.pop();

		if (current.mpNode == pTo)
		{
			toNodeAdded = true;
			break;
		}

		vector<Connection*> connections = mpGraph->getConnections(current.mpNode->getId());

		for (unsigned int i = 0; i < connections.size(); i++)
		{
			//gets the connections node data
			NodeRecord endNodeRecord;
			Node* endNode = connections[i]->getToNode();
			float endNodeCost = current.mCostSoFar + connections[i]->getCost() + getHeuristic(endNode, pTo);;
			float endNodeHeuristic = getHeuristic(endNode, pTo);
			bool closedContainsEndNode = false;
			bool openContainsEndNode = false;
			bool visitedNodesContainsEndNode = false;


			NodeRecord tempRecord = {};
			tempRecord.mpNode = endNode;
			//check to see if it is in the closed list, open list, or if its visited
			PriorityQueue<NodeRecord, vector<NodeRecord>, CompareEstimatedCost>::const_iterator iterVisited, iterToVisit;
			iterToVisit = openNodes.contains(tempRecord);
			iterVisited = closedNodes.contains(tempRecord);
			

			if (iterVisited != closedNodes.end())
			{
				endNodeRecord = *iterVisited;
				if (endNodeRecord.mCostSoFar <= endNodeCost)
				{
					continue;
				}
				closedNodes.remove(endNodeRecord);
				//mVisitedNodes.erase(iterVisited);
				//closedNodes.erase(closedNodes.begin()+closedNodeIndex);
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}
			else if (iterToVisit != openNodes.end())
			{
				visitedNodesContainsEndNode = true;
				endNodeRecord = *iterToVisit;
				if (endNodeRecord.mCostSoFar <= endNodeCost)
				{
					continue;

				}
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}
			else
			{
				endNodeRecord = NodeRecord();
				endNodeRecord.mpNode = endNode;
				endNodeHeuristic = getHeuristic(endNode, pTo);
			}


			endNodeRecord.mCostSoFar = endNodeCost;
			endNodeRecord.mpConnection = connections[i];
			endNodeRecord.mEstimatedTotalCost = endNodeCost + endNodeHeuristic;


			if (!openContainsEndNode && !toNodeAdded)
			{
				openNodes.push(endNodeRecord);
			}
		}
		//removes node from open list and puts it in the closed

		openNodes.remove(current);
		closedNodes.push(current);
#ifdef VISUALIZE_PATH
		mVisitedNodes.push_back(current.mpNode);
#endif
	}
	//create a vector of nodes for the final path

	Path* newPath = new Path();
	if (toNodeAdded == false)
	{
		return NULL;
	}
	else
	{
		while (current.mpNode != pFrom)
		{
			//finalNodeVector.push_back(current.mpNode);
			newPath->addNode(current.mpNode);
			current.mpNode = current.mpConnection->getFromNode();
			PriorityQueue<NodeRecord, vector<NodeRecord>, CompareCost>::const_iterator iter;
			iter = closedNodes.contains(current);

			if (iter != closedNodes.end())
			{
				current.mpConnection = iter->mpConnection;
			}
		}
		newPath->addNode(current.mpNode);
		
		//reverse the final node vector
		int pathSize = newPath->getNumNodes();
		for (int i = 0; i < pathSize; i++)
		{
			pPath->addNode(newPath->getAndRemoveNextNode());
		}
		delete newPath;
	}
	

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

NodeRecord AStarPathfinder::getSmallestNodeRecord(std::vector<NodeRecord> vNodeRecords)
{
	float lowestCost = INFINITY;
	int lowestNodeRecordIndex = 0;
	for (int i = 0; i < vNodeRecords.size(); i++)
	{
		if (vNodeRecords[i].mEstimatedTotalCost < lowestCost)
		{
			lowestCost = vNodeRecords[i].mEstimatedTotalCost;
			lowestNodeRecordIndex = i;
		}
	}
	return vNodeRecords[lowestNodeRecordIndex];
}

int AStarPathfinder::getSmallestNodeRecordIndex(std::vector<NodeRecord> vpNodeRecords)
{
	float lowestCost = INFINITY;
	int lowestNodeRecordIndex = 0;
	for (int i = 0; i < vpNodeRecords.size(); i++)
	{
		if (vpNodeRecords[i].mEstimatedTotalCost < lowestCost)
		{
			lowestCost = vpNodeRecords[i].mEstimatedTotalCost;
			lowestNodeRecordIndex = i;
		}
	}
	return lowestNodeRecordIndex;
}