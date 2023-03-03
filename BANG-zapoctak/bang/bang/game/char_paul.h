#ifndef PAUL_REGRET_H
#define PAUL_REGRET_H

#include "player.h"

//pri zahajeni hry se MU nastavi vzdalenost vsech o jedna VICE

class Paul : public Player {
public:
	Paul() : Player(15, 3, "PAUL REGRET") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
