#ifndef LUCKY_DUKE_H
#define LUCKY_DUKE_H

#include "player.h"

//override resolve_jail/dyn/barrel lize si na tyto karty 2 krat

class Lucky : public Player {
public:
    Lucky(Game* game) : Player(7, 4, game) {}
	virtual bool resolve_jail() override;
	virtual bool resolve_dyn() override;
	virtual bool resolve_barrel() override;
};

#endif
