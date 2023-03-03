#ifndef JOURDONNAIS_H
#define JOURDONNAIS_H

#include "player.h"

//funkce vedle, nebo skryty barel, jina karta nez obyc barel

class Jourd : public Player {
public:
	Jourd() : Player(4, 4, "JOURDONNAIS") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
