#include "FlowFieldPathFinder.h"
#include "Game.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include <list>
#include <PerformanceTracker.h>
#include "Connection.h"
#include "GridPathfinder.h"
#include "Path.h"

FlowFieldPathfinder::FlowFieldPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

FlowFieldPathfinder::~FlowFieldPathfinder()
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

void FlowFieldPathfinder::calculateIntegrationField(Node* pCenterNode)
{
	pCenterNode->setCost(0);
	std::list<Node*> nodesToVisit;

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	nodesToVisit.push_back(pCenterNode);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
#endif // VISUALIZE_PATH

	Path* pPath = new Path();
	Node* pCurrentNode = nullptr;
	bool toNodeAdded = false;

	while (nodesToVisit.size() > 0) {
		pCurrentNode = nodesToVisit.front();//access the top element
		nodesToVisit.pop_front(); //remove node, doesn't return it
		pPath->addNode(pCurrentNode);
		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		for (unsigned int i = 0; i < connections.size(); i++) {
			//for each neighbor of current node
			Connection* pConnection = connections[i];

			//set up node.
			Node* pTempToNode = connections[i]->getToNode();
			auto cost = pConnection->getCost() + pCurrentNode->getCost();

			if (!pPath->containsNode(pTempToNode) &&
				find(nodesToVisit.begin(), nodesToVisit.end(), pTempToNode) == nodesToVisit.end())
			{
				pTempToNode->setCost(cost);
				nodesToVisit.push_back(pTempToNode);

#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNode);
#endif
			}
		}
	}
	delete pPath;
}

Path* FlowFieldPathfinder::findPath(Node* pFrom, Node* pTo)
{
	calculateIntegrationField(pTo);
	return mpPath;
}