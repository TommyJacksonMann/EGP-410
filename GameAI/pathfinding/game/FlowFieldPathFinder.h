#pragma once
#include "GridPathfinder.h"
class Node;

//Class pathfinding for flow feild. Class made by Thomas Mann

class FlowFieldPathfinder : public GridPathfinder {
public:
	FlowFieldPathfinder(Graph* pGraph);
	~FlowFieldPathfinder();
		
	//Calculates the cost for all the nodes
	void calculateIntegrationField(Node* pCenterNode);	
	
	//calculates all the connections for the nodes
	void calculateFlowField();

	Path* findPath(Node* pFrom, Node* pTo);
private:
	Node * mpCenterNode;

};