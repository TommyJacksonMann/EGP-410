#pragma once
#include "GridPathfinder.h"
class Node;

class FlowFieldPathFinder : public GridPathfinder {
public:
	FlowFieldPathFinder(Graph* pGraph);
	~FlowFieldPathFinder();

	void calculateIntegrationField(Node* pCenterNode);
private:
	Node * mpCenterNode;

};