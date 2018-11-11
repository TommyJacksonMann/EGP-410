#pragma once
#include "Trackable.h"

class Node;

class FlowFieldPathFinder : public Trackable {
public:
	FlowFieldPathFinder();
	~FlowFieldPathFinder();

	void calculateIntegrationField(Node* pNode);
private:
	Node * mpCenterNode;

};