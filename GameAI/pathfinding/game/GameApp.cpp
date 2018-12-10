#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "DijkstraPathFinder.h"
#include "AStarPathFinder.h"
#include "FlowFieldPathFinder.h"
#include "InputSystem.h"
#include "PathPool.h"
#include "StateMachingFiles/StateMachine.h"
#include "./StateMachingFiles/PlayerAttackState.h"
#include "SpawnItemMessage.h"
#include "DataParser.h"
#include "SpawnEnemyMessage.h"

#include <SDL.h>
#include <fstream>
#include <vector>

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	mpInputSystem = new InputSystem();
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();


	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	//mpPathfinder = new DijkstraPathfinder(mpGridGraph);
	mpPathfinder = new AStarPathfinder(mpGridGraph);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	mpPathPool = new PathPool();

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );


	mMaxCoinsOnScreen = DataParser::getInstance()->ReadFile("MaxCoins");
	mMaxPowerUpsOnScreen = DataParser::getInstance()->ReadFile("MaxPowerUps");

	mCurrentCoinsOnScreen = 0;
	mCurrentPowerUpsOnScreen = 0;

	mPlayerAttackTime = DataParser::getInstance()->ReadFile("PowerUpTime");
 	mPlayerSpeed = DataParser::getInstance()->ReadFile("PlayerSpeed");

	mEnemySpeed = DataParser::getInstance()->ReadFile("EnemySpeed");
	mEnemyChaseRange = DataParser::getInstance()->ReadFile("EnemyChaseRange");
	mEnemyChangeDirectionFrequency = DataParser::getInstance()->ReadFile("EnemyRandomFrequency");

	mPowerUpDelay = DataParser::getInstance()->ReadFile("PowerUpSpawnTime");
	mPowerUpFrequency = DataParser::getInstance()->ReadFile("PowerUpSpawnFrequency");

	mCoinFrequency = DataParser::getInstance()->ReadFile("CoinSpawnFrequency");
	mCoinDelay = DataParser::getInstance()->ReadFile("CoinSpawnTime");

	mEnemySpawnOne = Vector2D(DataParser::getInstance()->ReadFile("EnemySpawnOneX"), DataParser::getInstance()->ReadFile("EnemySpawnOneY"));
	mEnemySpawnTwo = Vector2D(DataParser::getInstance()->ReadFile("EnemySpawnTwoX"), DataParser::getInstance()->ReadFile("EnemySpawnTwoY"));
	mEnemySpawnThree = Vector2D(DataParser::getInstance()->ReadFile("EnemySpawnThreeX"), DataParser::getInstance()->ReadFile("EnemySpawnThreeY"));
	mEnemySpawnFour = Vector2D(DataParser::getInstance()->ReadFile("EnemySpawnFourX"), DataParser::getInstance()->ReadFile("EnemySpawnFourY"));

	mEnemySpawnFrequency = DataParser::getInstance()->ReadFile("EnemySpawnFrequency");
	mEnemySpawnTime = DataParser::getInstance()->ReadFile("EnemySpawnTime");
	mMaxEnemies = DataParser::getInstance()->ReadFile("MaxEnemies");

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpPathPool;
	mpPathPool = NULL;

	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	delete mpPathfinder;
	mpPathfinder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpInputSystem;
	mpInputSystem = NULL;

}

void GameApp::beginLoop()
{

	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw( *pBackBuffer );

	mpDebugDisplay->draw( pBackBuffer );
	//mpGridGraph->VisualizeNodeCosts(); //UNCOMMENT THIS TO SEE NODE COSTS AND NODE DIRECTIONS, FRAME RATE IS CHOO

	mpInputSystem->update();


	mpMessageManager->processMessagesForThisframe();

	//get input - should be moved someplace better
	SDL_PumpEvents();

	
	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{

	GameMessage* pItemMessage = new SpawnItemMessage();
	mpMessageManager->addMessage(pItemMessage, 0);
	GameMessage* pEnemySpawnMessage = new SpawnEnemyMessage();
	mpMessageManager->addMessage(pEnemySpawnMessage, 0);
	return Game::endLoop();
}

void GameApp::SetPathFinderToDijkstra() {
	delete mpPathfinder;
	mpPathfinder = new DijkstraPathfinder(mpGridGraph);

	delete mpDebugDisplay;

	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}
void GameApp::SetPathFinderToAStar() {
	delete mpPathfinder;
	mpPathfinder = new AStarPathfinder(mpGridGraph);

	delete mpDebugDisplay;

	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}
void GameApp::SetPathFinderToDepthFirst() {
	delete mpPathfinder;
	mpPathfinder = new DepthFirstPathfinder(mpGridGraph);

	delete mpDebugDisplay;

	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}
void GameApp::SetPathFinderToFlowField() {
	delete mpPathfinder;
	mpPathfinder = new FlowFieldPathfinder(mpGridGraph);

	delete mpDebugDisplay;

	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::AddScore(ScoreType type)
{
	switch(type)
	{
		case COIN_SCORE:
		{
			mGameScore += mCoinScore;
			break;
		}
		case EAT_ENEMY_SCORE:
		{
			mGameScore += mPlayerEat;
			break;
		}
		case NO_SCORE:
		{
		}
		default:
		{}
	};
}
