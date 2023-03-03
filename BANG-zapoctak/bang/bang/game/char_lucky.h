#ifndef LUCKY_DUKE_H
#define LUCKY_DUKE_H

#include "player.h"

//private func

class Lucky : public Player {
public:
	Lucky() : Player(8, 4, "LUCKY DUKE") {};
	virtual void game_phase() override
	{

	}
	virtual void discard_phase() override
	{

	}
private:
	virtual bool resolve_jail(Game* g) override;
	virtual bool resolve_dyn(Game* g) override;
};




#endif
