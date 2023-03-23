#ifndef ROSE_DOOLAN_H
#define ROSE_DOOLAN_H

#include "player.h"

//pri zahajeni hry se ji nastavi vzdalenost vsech o jedna mene

class Rose : public Player {
public:
    Rose(Game* game) : Player(13, 4, "rose", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
