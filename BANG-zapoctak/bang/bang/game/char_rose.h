#ifndef ROSE_DOOLAN_H
#define ROSE_DOOLAN_H

#include "player.h"

//pri zahajeni hry se ji nastavi vzdalenost vsech o jedna mene

class Rose : public Player {
public:
	Rose() : Player(13, 4, "ROSE DOOLAN") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
