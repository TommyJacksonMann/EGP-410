#pragma once
#include "Trackable.h"
#include "Node.h"
#include "Connection.h"

enum Category
{
	UNVISITED,
	OPEN,
	CLOSED
};

struct NodeRecord : public Trackable {
	NodeRecord() : mpNode(NULL), mpConnection(NULL), mCostSoFar(0), mEstimatedTotalCost(0), mCategory(UNVISITED) {};
	NodeRecord(Node* pNode, Connection* pConnection, float cost, float estimatedCost, Category category)
		: mpNode(pNode), mpConnection(pConnection), mCostSoFar(cost), mEstimatedTotalCost(estimatedCost), mCategory(category) {};
	~NodeRecord() { }

	friend bool operator== (const NodeRecord &lhs, const NodeRecord &rhs) { return  lhs.mpNode == rhs.mpNode; };
	bool operator ==(const NodeRecord& rhs) { return mpNode == rhs.mpNode; };
	Node* mpNode;
	Connection* mpConnection;
	float mCostSoFar;/**/
	float mEstimatedTotalCost;
	Category mCategory;
};