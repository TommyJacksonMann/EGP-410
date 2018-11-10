#pragma once

#include "Pathfinder.h"
class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;
class Node;
class Connection;

class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path* findPath( Node* pFrom, Node* pTo ) = 0;

#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );

	//Draws the path for any other unit that might need its path drawn
	void drawOtherPath(Grid* pGrid, GraphicsBuffer* pDest, Path path);
protected:
	std::vector<Node*> mVisitedNodes;
	Path* mpPath;
	GridVisualizer* mpVisualizer;
#endif

	double mTimeElapsed;
};