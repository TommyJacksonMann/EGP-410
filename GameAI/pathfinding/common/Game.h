#pragma once

/* Game - base class for actual application class

	Dean Lawson
	Champlain College
	2011
*/
#include <Trackable.h>
#include <Timer.h>
#include "Defines.h"

class MemoryTracker;
class PerformanceTracker;
class GraphicsSystem;
class GraphicsBuffer;
class GraphicsBufferManager;
class SpriteManager;
class Game;
class Font;
class InputSystem;
class ComponentManager;
class UnitManager;
class StateTransition;

extern PerformanceTracker* gpPerformanceTracker;
extern Game* gpGame;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType NODE_DIRECTION_SPRITE_ID = 4;
const IDType COIN_SPRITE_ID = 5;
const IDType PLAYER_ATTACK_ICON_SPRITE_ID = 6;
const IDType POWER_UP_SPRITE_ID = 7;

const float LOOP_TARGET_TIME = 33.3f;

class Game:public Trackable
{
public:
	Game();
	virtual ~Game();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline ComponentManager* getComponentManager() const { return mpComponentManager;  };
	inline UnitManager* getUnitManager() const { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline Font* getFont() const { return mpFont; };
	inline void markForExit() { mShouldExit = true; };

	StateTransition* getWanderTrans() { return mpToAiWanderTrans; }
	StateTransition* getChaseTrans() { return mpToAiChaseTrans; }
	StateTransition* getFleeTrans() { return mpToAiFleeTrans; }


protected:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	float mLoopTargetTime;
	bool mShouldExit;

	StateTransition* mpToPlayerRunTrans;
	StateTransition * mpToPlayerAttackTrans;
	StateTransition* mpToPlayerAiRunTrans; 
	StateTransition* mpToPlayerAiAttackTrans; 

	StateTransition* mpToAiWanderTrans;
	StateTransition* mpToAiFleeTrans;
	StateTransition* mpToAiChaseTrans;

	
	Font* mpFont;

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "background";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mPlayerAttackIconBufferID = "player-attack";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mNodeDirectionBufferID = "nodedirection";
	GraphicsBufferID mCoinBufferID = "coin";
	GraphicsBufferID mPowerUpID = "power-up";

};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge
float mapRotationToRange( float rotation, float low, float high );

const double PI = std::atan(1.0)*4;//could proved useful!
