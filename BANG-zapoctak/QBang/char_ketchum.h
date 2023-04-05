#ifndef SID_KETCHUM_H
#define SID_KETCHUM_H

#include "player.h"

//uvnitr discard_phase odhodi karty pokud mu to pomuze

class Ketchum : public Player {
public:
    Ketchum(Game* game) : Player(6, 4, "ketchum", game) {}
	virtual void discard_phase() override;
    virtual void draw_phase() override;
    virtual void ability() override;
};

#endif
