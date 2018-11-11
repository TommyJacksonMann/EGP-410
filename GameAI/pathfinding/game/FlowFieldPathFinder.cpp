#include "FlowFieldPathFinder.h"
#include "Game.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"

FlowFieldPathFinder::FlowFieldPathFinder()
{
	
}

FlowFieldPathFinder::~FlowFieldPathFinder()
{
}

void FlowFieldPathFinder::calculateIntegrationField(Node* pNode)
{
	mpCenterNode = pNode;
}