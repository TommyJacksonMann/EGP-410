#pragma once

#include <Trackable.h>

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	PATH_TO_MESSAGE = 1,
	ESCAPE_MESSAGE,
	DIJKSTRA_MESSAGE,
	A_STAR_MESSAGE,
	DEPTH_FIRST_SEARCH_MESSAGE,
	ADD_AND_REMOVE_MESSAGE,
	UNIT_TO_NEW_LOCATION_MESSAGE,
	HANDLE_COLLISION_MESSAGE,
	SPAWN_COIN_MESSAGE,
	DELETE_COINS_MESSAGE,
	RESET_PLAYER_POS_MESSAGE,
	RESET_GAME_MESSAGE,
	PLAYER_RUN_MESSAGE,
	PLAYER_ATTACK_MESSAGE,
	CHANGE_PLAYER_CONTROL_TYPE_MESSAGE,
	SPAWN_ENEMY_MESSAGE,
	DELETE_ENEMY_MESSAGE
};

class GameMessage: public Trackable
{
public:
	friend class GameMessageManager;

	GameMessage( MessageType type  );
	~GameMessage();

	double getSubmittedTime() const { return mSubmittedTime; };
	double getScheduledTime() const { return mScheduledTime; };

protected:
	MessageType mType;
	double mSubmittedTime;
	double mScheduledTime;

private:
	virtual void process() = 0; 
};


