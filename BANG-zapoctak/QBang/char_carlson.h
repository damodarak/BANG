#ifndef KIT_CARLSON_H
#define KIT_CARLSON_H

#include "player.h"

//override draw

class Carlson : public Player {
public:
    Carlson(Game* game) : Player(8, 4, "carlson", game) {};
	virtual void draw_phase() override
	{

	}
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
