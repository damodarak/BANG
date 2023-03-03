#ifndef WILLY_THE_KID_H
#define WILLY_THE_KID_H

#include "player.h"

class Willy : public Player {
public:
	Willy() : Player(1, 4, "WILLY THE KID") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
