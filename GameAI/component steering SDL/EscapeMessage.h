#pragma once
#include "GameMessage.h"

class EscapeMessage :public GameMessage
{
public:
	EscapeMessage();
	~EscapeMessage();

	void process();
};