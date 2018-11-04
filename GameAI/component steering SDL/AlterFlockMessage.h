#pragma once
#include "GameMessage.h"

enum FlockData {
	ALIGN,
	COHESION,
	SEPARATION
};

class AlterFlockMessage :public GameMessage
{
public:
	AlterFlockMessage(FlockData data, float value);
	~AlterFlockMessage();

	void process();

private:
	FlockData mDataToAlter;
	float mValue = 0;
};