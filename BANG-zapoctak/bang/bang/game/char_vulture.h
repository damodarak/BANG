#ifndef VULTURE_SAM_H
#define VULTURE_SAM_H

#include "player.h"

//karty mrtveho mu da hra

class Vulture : public Player {
public:
	Vulture() : Player(9, 4, "VULTURE SAM") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif