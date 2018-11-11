#pragma once
#include "GridPathfinder.h"
class Node;

class FlowFieldPathfinder : public GridPathfinder {
public:
	FlowFieldPathfinder(Graph* pGraph);
	~FlowFieldPathfinder();

	void calculateIntegrationField(Node* pCenterNode);
	void calculateFlowField();
	Path* findPath(Node* pFrom, Node* pTo);
private:
	Node * mpCenterNode;

};