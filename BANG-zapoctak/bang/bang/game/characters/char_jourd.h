#ifndef JOURDONNAIS_H
#define JOURDONNAIS_H

#include "../player.h"

//funkce vedle, nebo skryty barel, jina karta nez obyc barel

class Jourd : public Player {
public:
	Jourd(Game* game) : Player(4, 4, "JOURDONNAIS", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
