#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

DijkstraPathfinder::DijkstraPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

DijkstraPathfinder::~DijkstraPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* DijkstraPathfinder::findPath(Node* pFrom, Node* pTo)
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
	NodeRecord* current;
	vector<NodeRecord*> openNodes;
	openNodes.push_back(startRecord);
	vector<NodeRecord*> closedNodes;

	while (openNodes.size() > 0)
	{
		current = openNodes[0];
		int currentNodeIndex = 0;
		if (current->mpNode == pTo)
		{
			toNodeAdded = true;
			break;
		}

		vector<Connection*> connections = mpGraph->getConnections(current->mpNode->getId());
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			Node* endNode = connections[i]->getToNode();
			float endNodeCost = current->mCostSoFar + connections[i]->getCost();
			//check to see if it is in the closed list
			bool closedContainsEndNode = false;
			bool openContainsEndNode = false;
			NodeRecord* endNodeRecord;
			for (unsigned int i = 0; i < closedNodes.size(); i++)
			{
				if (closedNodes[i]->mpNode == endNode)
				{
					closedContainsEndNode = true;
				}
			}
			for (unsigned int i = 0; i < openNodes.size(); i++)
			{
				if (openNodes[i]->mpNode == endNode)
				{
					openContainsEndNode = true;
					endNodeRecord = openNodes[i];
				}
			}
			if (closedContainsEndNode) {}
			else if (openContainsEndNode)
			{
				if (endNodeRecord->mCostSoFar <= endNodeCost) {}
			}
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->mpNode = endNode;
				endNodeRecord->mCostSoFar = endNodeCost;
				endNodeRecord->mpConnection = connections[i];
				if (!openContainsEndNode)
				{
					openNodes.push_back(endNodeRecord);
				}
			}
		}
		
		openNodes.erase(openNodes.begin()+ currentNodeIndex);
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

	for (unsigned int i = 0; i < closedNodes.size(); i++)
	{
		delete closedNodes[i];
	}
	for (unsigned int i = 0; i < openNodes.size(); i++)
	{
		delete openNodes[i];
	}

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif
	return pPath;

}