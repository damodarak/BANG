#ifndef PEDRO_RAMIREZ_H
#define PEDRO_RAMIREZ_H

#include "player.h"

//override draw_phase
//pokud je karta na spodu balicku zajimava, tak si ji vezme
//notai muze pouzit tuto schopnost zmacknutim Ability tlacitka

class Pedro : public Player {
public:
    Pedro(Game* game) : Player(9, 4, game) {}
    virtual void draw_phase() override;
    virtual void ability() override;
};

#endif
