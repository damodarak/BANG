#ifndef SID_KETCHUM_H
#define SID_KETCHUM_H

#include "player.h"

//uvnitr game_phase nebo discard_phase

class Ketchum : public Player {
public:
	Ketchum() : Player(6, 4, "SID KETCHUM") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif
