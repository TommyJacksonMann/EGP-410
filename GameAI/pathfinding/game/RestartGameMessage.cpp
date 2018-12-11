#include "RestartGameMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "DeleteAllCoinsMessage.h"
#include "DeleteAllEnemiesMessage.h"
#include "ResetPlayerStartPosMessage.h"
#include "GameMessageManager.h"



RestartGameMessage::RestartGameMessage(bool restartGame)
	: mResetOrRestart(restartGame), GameMessage(RESET_GAME_MESSAGE)
{
}


RestartGameMessage::~RestartGameMessage()
{
}

void RestartGameMessage::process()
{
	GameApp* pGame = static_cast<GameApp*>(gpGame);
	if (mResetOrRestart)
	{
		pGame->AddScore(NO_SCORE);
		pGame->setPlayerCurrentLives(pGame->getPlayerStartLives());

		//RESET TIMES FOR EVERY FREQUENCY 
	}

	if (pGame->getPlayerLives() < 0)
	{
		//EndGame
		pGame->setGameEnd();
	}

	GameMessage* pDeleteCoins = new DeleteAllCoinsMessage();
	pGame->getMessageManager()->addMessage(pDeleteCoins, 0);
	GameMessage* pDeleteEnemies = new DeleteAllEnemiesMessage();
	pGame->getMessageManager()->addMessage(pDeleteEnemies, 0);
	GameMessage* pResetPos = new ResetPlayerStartPos();
	pGame->getMessageManager()->addMessage(pResetPos, 0);

}