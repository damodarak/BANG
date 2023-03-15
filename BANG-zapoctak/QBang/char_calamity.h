#ifndef CALAMITY_JANET_H
#define CALAMITY_JANET_H

#include "player.h"

//resolve bang,vedle easy....bang easy.... vedle muze strilet, tot vse..coz ma smysl jenom pokud na nema co hrat ma vedle a nehrala bang jeste

class Calamity : public Player {
public:
	Calamity(Game* game) : Player(2, 4, "CALAMITY JANET", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
