#ifndef WILLY_THE_KID_H
#define WILLY_THE_KID_H

#include "player.h"

class Willy : public Player {
public:
	Willy(int max_healt) : max_healt_(max_healt) {};
	virtual void draw_phase() override
	{

	}
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
private:
	int max_healt_;
};




#endif
