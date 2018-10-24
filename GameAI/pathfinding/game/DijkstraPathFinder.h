#pragma once

#include "GridPathfinder.h"
#include <vector>
#include "NodeRecord.h"


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

class DijkstraPathfinder :public GridPathfinder
{
public:
	DijkstraPathfinder(Graph* pGraph);
	~DijkstraPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:
	
};

