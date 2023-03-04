#ifndef SUZY_LAFAYETTE_H
#define SUZY_LAFAYETTE_H

#include "../player.h"

//po kazde vyresolvovane udalosti se hra zepta suze, zda-li ma kartu

class Suzy : public Player {
public:
	Suzy(Game* game) : Player(14, 4, "SUZE LAFAZETTE", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif