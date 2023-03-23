#ifndef SID_KETCHUM_H
#define SID_KETCHUM_H

#include "player.h"

//uvnitr discard_phase

class Ketchum : public Player {
public:
    Ketchum(Game* game) : Player(6, 4, "ketchum", game) {};
	virtual void discard_phase() override;
};


#endif
