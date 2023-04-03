#ifndef JESSE_JONES_H
#define JESSE_JONES_H

#include "player.h"

//override draw_phase, ma-li nepratele tak si od neho vezme kartu

class Jesse : public Player {
public:
    Jesse(Game* game) : Player(11, 4, "jesse", game) {}
    virtual void draw_phase() override;
    virtual void ability() override;
};




#endif
