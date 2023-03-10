#ifndef LUCKY_DUKE_H
#define LUCKY_DUKE_H

#include "../player.h"

//private func

class Lucky : public Player {
public:
	Lucky(Game* game) : Player(8, 4, "LUCKY DUKE", game) {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
	virtual bool resolve_jail() override;
	virtual bool resolve_dyn() override;
};




#endif
