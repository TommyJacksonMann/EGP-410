#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include <vector>


//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputSystem;
class DijkstraPathFinder;
class AStarPathFinder;
class PathPool;

enum ScoreType 
{
COIN_SCORE,
EAT_ENEMY_SCORE,
NO_SCORE
};

//const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline InputSystem* getInputSystem() const { return mpInputSystem; }
	inline PathPool* getPathPool() const { return mpPathPool; }
	void SetPathFinderToDijkstra();
	void SetPathFinderToAStar();
	void SetPathFinderToDepthFirst();
	void SetPathFinderToFlowField();

	const int getMaxCoinsOnScreen() const { return mMaxCoinsOnScreen; }
	const int getCurrentCoinsOnScreen() const { return mCurrentCoinsOnScreen; }
	
	void setCoinsOnScreen(const int newAmount) { mCurrentCoinsOnScreen = newAmount; }
	
	void AddScore(ScoreType);
private:

	InputSystem* mpInputSystem;
	GameMessageManager* mpMessageManager;
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	DebugDisplay* mpDebugDisplay;

	GridPathfinder* mpPathfinder;

	PathPool* mpPathPool;

	int mGameScore = 0;
	int mCoinScore = 0;
	int mPlayerEat = 0;

	int mMaxCoinsOnScreen = 1;
	int mCurrentCoinsOnScreen = 1;
};

