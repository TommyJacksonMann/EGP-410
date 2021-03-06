
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Font.h"
#include <Timer.h>
#include "Defines.h"
#include "..\game\SteeringFiles\UnitManager.h"
#include "..\game\SteeringFiles\Unit.h"
#include "..\game\SteeringFiles\ComponentManager.h"
#include "..\game\DataParser.h"


Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	, mpGraphicsBufferManager(NULL)
	, mpSpriteManager(NULL)
	, mpLoopTimer(NULL)
	, mpMasterTimer(NULL)
	, mShouldExit(false)
	, mpFont(NULL)
	, mLoopTargetTime(0.0f)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	srand((UINT)time(0));//seed random number generator

	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;


	DataParser::initInstance();


	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init(WIDTH, HEIGHT);
	if (!goodGraphics)
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();


	//load background
	mpGraphicsBufferManager->loadBuffer(mCoinBufferID, "coin.png");
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, "player.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, "enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID, "target.png");
	mpGraphicsBufferManager->loadBuffer(mNodeDirectionBufferID, "flow-field-direction.png");
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer(mBackgroundBufferID);
	if (pBackGroundBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight());
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPlayerIconBufferID);
	Sprite* pArrowSprite = NULL;
	if (pPlayerBuffer != NULL)
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight());
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemySprite = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemySprite = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}
	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}
	GraphicsBuffer* pNodeDirectionBuffer = mpGraphicsBufferManager->getBuffer(mNodeDirectionBufferID);
	Sprite* pNodeDirection = NULL;
	if (pNodeDirectionBuffer != NULL)
	{
		pNodeDirection = mpSpriteManager->createAndManageSprite(NODE_DIRECTION_SPRITE_ID, pNodeDirectionBuffer, 0, 0, (float)pNodeDirectionBuffer->getWidth(), (float)pNodeDirectionBuffer->getHeight());
	}
	GraphicsBuffer* pCoinBuffer = mpGraphicsBufferManager->getBuffer(mCoinBufferID);
	Sprite* pCoinSprite = NULL;
	if (pCoinBuffer != NULL)
	{
		pCoinSprite = mpSpriteManager->createAndManageSprite(COIN_SPRITE_ID, pCoinBuffer, 0, 0, (float)pCoinBuffer->getWidth(), (float)pCoinBuffer->getHeight());
	}

	//load Font
	mpFont = new Font("cour.ttf", 24);

	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);
	

	Vector2D playerSpawn(DataParser::getInstance()->ReadFile("PlayerSpawnX"), DataParser::getInstance()->ReadFile("PlayerSpawnY"));
	Unit* pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite);
	pUnit->setShowTarget(false);
	pUnit->getPositionComponent()->setPosition(playerSpawn);
	pUnit->setSteering(Steering::KINEMATICARRIVE, playerSpawn);
	pUnit->setCollision(CIRCLE, 5);
	pUnit->setUnitType(UnitType::PLAYER);

	Unit* testEnemy = mpUnitManager->createUnit(*pEnemySprite);
	testEnemy->getPositionComponent()->setPosition(Vector2D(10 * 32, 5 * 32));
	testEnemy->setCollision(CIRCLE, 5);
	testEnemy->setUnitType(UnitType::ENEMY);
	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//cleanup font
	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
	

	DataParser::cleanUpInstance();
}

void Game::beginLoop()
{
	mpLoopTimer->start();

	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	mpGraphicsSystem->draw(*pBackgroundSprite, 0.0f, 0.0f);
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;

void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	mpUnitManager->drawAll();
	mpGraphicsSystem->swap();
	std::cout << mpLoopTimer->getElapsedTime() << std::endl;

}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed(mLoopTargetTime);
	return mShouldExit;
}


float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand() / (float)RAND_MAX;
	return r;
}

float mapRotationToRange(float rotation, float low, float high)
{
	while (rotation < low)
	{
		rotation += (2.0f * (float)PI);
	}

	while (rotation > high)
	{
		rotation -= (2.0f * (float)PI);
	}
	return rotation;
}
