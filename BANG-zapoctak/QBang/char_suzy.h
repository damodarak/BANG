#ifndef SUZY_LAFAYETTE_H
#define SUZY_LAFAYETTE_H

#include "player.h"

//po kazde vyresolvovane udalosti se hra zepta suze, zda-li ma kartu

class Suzy : public Player {
public:
    Suzy(Game* game) : Player(14, 4, "suzy", game) {}
    virtual int game_phase() override;
    virtual Card give_random_card() override;
    virtual Card give_random_card_hand() override;
    virtual void ability() override;
};




#endif
