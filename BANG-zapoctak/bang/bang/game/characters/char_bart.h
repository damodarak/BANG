#ifndef BART_CASSIDY_H
#define BART_CASSIDY_H

#include "../player.h"

//sam si lizne vzdy

class Bart : public Player {
public:
	Bart(Game* game) : Player(12, 4, "BART CASSIDY", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif