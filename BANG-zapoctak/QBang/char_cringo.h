#ifndef EL_CRINGO_H
#define EL_CRINGO_H

#include "player.h"

//v resolve action

class Cringo : public Player {
public:
	Cringo(Game* game) : Player(16, 3, "EL CRINGO", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
