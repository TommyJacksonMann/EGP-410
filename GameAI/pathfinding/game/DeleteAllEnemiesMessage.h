#pragma once
#include "GameMessage.h"
class DeleteAllEnemiesMessage :
	public GameMessage
{
public:
	DeleteAllEnemiesMessage();
	~DeleteAllEnemiesMessage();
	
	void process();


};

