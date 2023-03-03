#ifndef BART_CASSIDY_H
#define BART_CASSIDY_H

#include "player.h"

//sam si lizne vzdy

class Bart : public Player {
public:
	Bart() : Player(12, 4, "BART CASSIDY") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
};




#endif