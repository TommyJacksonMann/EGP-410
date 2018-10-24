#pragma once

#include "GridPathfinder.h"
#include <vector>
#include "NodeRecord.h"

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

class AStarPathfinder :public GridPathfinder
{
public:
	AStarPathfinder(Graph* pGraph);
	~AStarPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!
private:
	float getHeuristic(Node* pFrom, Node* pTo);
	NodeRecord* getSmallestNodeRecord(std::vector<NodeRecord*> vpNodeRecords);	
	int getSmallestNodeRecordIndex(std::vector<NodeRecord*> vpNodeRecords);
};

