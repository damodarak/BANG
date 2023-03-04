#ifndef SLAB_THE_KILLER_H
#define SLAB_THE_KILLER_H

#include "../player.h"

//slab bude sam davat miss2 do actionu nebo tak neco nebo hra

class Slab : public Player {
public:
	Slab(Game* game) : Player(3, 4, "SLAB THE KILLER", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif