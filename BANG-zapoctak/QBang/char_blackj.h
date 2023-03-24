#ifndef BLACK_JACK_H
#define BLACK_JACK_H

#include "player.h"

//draw phase

class Blackj : public Player {
public:
    Blackj(Game* game) : Player(5, 4, "blackj", game) {};
	virtual void draw_phase() override
	{

	}
};




#endif
