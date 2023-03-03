#ifndef BLACK_JACK_H
#define BLACK_JACK_H

#include "player.h"

//draw phase

class Blackj : public Player {
public:
	Blackj() : Player(5, 4, "BLACK JACK") {};
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