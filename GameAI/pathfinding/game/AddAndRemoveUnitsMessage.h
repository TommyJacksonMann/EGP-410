#pragma once
#include "GameMessage.h"

//Class for Adding 10 units to the scene and removing the old ones. Also sets the pathfinding for each Unit. Created by RJ Bourdelais

class AddAndRemoveUnitsMessage :
	public GameMessage
{
public:
	AddAndRemoveUnitsMessage();
	~AddAndRemoveUnitsMessage();

	void process();
};

