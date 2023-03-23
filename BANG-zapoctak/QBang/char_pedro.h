#ifndef PEDRO_RAMIREZ_H
#define PEDRO_RAMIREZ_H

#include "player.h"

//override draw_phase

class Pedro : public Player {
public:
    Pedro(Game* game) : Player(10, 4, "pedro", game) {};
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
