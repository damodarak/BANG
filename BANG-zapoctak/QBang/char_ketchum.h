#ifndef SID_KETCHUM_H
#define SID_KETCHUM_H

#include "player.h"

//uvnitr discard_phase

class Ketchum : public Player {
public:
    Ketchum(Game* game) : Player(6, 4, "ketchum", game) {}
	virtual void discard_phase() override;
    virtual void draw_phase() override { discarded = 0;}
    virtual void ability() override;
};


#endif
