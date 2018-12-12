#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include <vector>
#include <Vector2D.h>


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
//class Vector2D;

enum ScoreType 
{
COIN_SCORE,
EAT_ENEMY_SCORE,
NO_SCORE
};

//const double LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

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
	const int getMaxPowerUpsOnScreen() const { return mMaxPowerUpsOnScreen; }
	const int getCurrentPowerUpsOnScreen() const { return mCurrentPowerUpsOnScreen; }


	void setCoinsOnScreen(const int newAmount) { mCurrentCoinsOnScreen = newAmount; }
	void setPowerUpsOnScreen(const int newAmount) { mCurrentPowerUpsOnScreen = newAmount; }

	const double getPowerUpDelay() const { return mPowerUpDelay; }
	const int getPowerUpFrequency() const { return mPowerUpFrequency; }

	const double getCoinDelay() const { return mCoinDelay; }
	const int getCoinFrequency() const { return mCoinFrequency; }

	void setLastCoinFired(const double newTime) { mLastTimeFiredCoin = newTime; }
	void setLastPowerUpFired(const double newTime) { mLastTimeFiredPowerUp = newTime; }
	const double getLastCoinFired() const { return mLastTimeFiredCoin; }
	const double getLastPowerUpFired() const { return mLastTimeFiredPowerUp; }

	const int getPlayerAttackTime() const { return mPlayerAttackTime; }

	const float getPlayerSpeed() const { return mPlayerSpeed; }
	const float getEnemySpeed() const { return mEnemySpeed; }
	
	const float getEnemyChaseRange() const { return mEnemyChaseRange; }
	const float getEnemyDirectionFrquency() const { return mEnemyChangeDirectionFrequency; }

	 Vector2D getEnemySpawnOne()  { return mEnemySpawnOne; }
	 Vector2D getEnemySpawnTwo()  { return mEnemySpawnTwo; }
	 Vector2D getEnemySpawnThree()  { return mEnemySpawnThree; }
	 Vector2D getEnemySpawnFour()  { return mEnemySpawnFour; }

	const int getEnemySpawnFrequency() const { return mEnemySpawnFrequency; }
	const float getLastEnemySpawned() const { return mLastEnemySpawned; }
	void setLastEnemySpawned(const float time) { mLastEnemySpawned = time; }
	const float getEnemySpawnTime() const { return mEnemySpawnTime; }

	const int getCurrentEnemies() const { return mCurrentEnemies; }
	void setCurrentEnemies(const int newNum) { mCurrentEnemies = newNum; }
	const int getMaxEnemies() const { return mMaxEnemies; }

	const int getPlayerLives() const { return mPlayerCurrentLives; }
	void setPlayerCurrentLives(const int newLives) { mPlayerCurrentLives = newLives; }
	const int getPlayerStartLives() const { return mPlayerStartLives; }

	const int getHighScore() const { return mHighScore; }
	void setHighScore(const int newHighScore) { mHighScore = newHighScore; }

	const int getScore() const { return mGameScore; }
	void AddScore(ScoreType);
	void drawScore();
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

	int mHighScore = 0;
	
	int mCoinsCollected = 0;
	int mEnemiesEaten = 0;

	int mPlayerAttackTime = 0;
	float mPlayerSpeed = 0;
	int mPlayerStartLives = 0;
	int mPlayerCurrentLives = 0;

	float mEnemySpeed = 0;
	float mEnemyChaseRange = 0;
	float mEnemyChangeDirectionFrequency = 0;

	int mMaxCoinsOnScreen = 1;
	int mCurrentCoinsOnScreen = 1;
	int mMaxPowerUpsOnScreen = 0;
	int mCurrentPowerUpsOnScreen = 0;
	double mLastTimeFiredCoin = 0;
	double mLastTimeFiredPowerUp = 0;
	
	double mCoinDelay = 0;
	double mPowerUpDelay = 0;
	int mCoinFrequency = 0;
	int mPowerUpFrequency = 0;

	Vector2D mEnemySpawnOne;
	Vector2D mEnemySpawnTwo;
	Vector2D mEnemySpawnThree;
	Vector2D mEnemySpawnFour;

	int mEnemySpawnFrequency = 0;
	float mLastEnemySpawned = 0;
	float mEnemySpawnTime = 0;

	int mCurrentEnemies = 0;
	int mMaxEnemies = 0;

};

