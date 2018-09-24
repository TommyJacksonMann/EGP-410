#include "EscapeMessage.h"
#include "Game.h"


EscapeMessage::EscapeMessage()
	:GameMessage(ESCAPE_MESSAGE)
{

}

EscapeMessage::~EscapeMessage()
{

}

void EscapeMessage::process()
{
	gpGame->exitGame();
}