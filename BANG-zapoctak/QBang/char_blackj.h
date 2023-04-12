#ifndef BLACK_JACK_H
#define BLACK_JACK_H

#include "player.h"

//2. karta karova nebo srdcova => lize si 3. kartu

class Blackj : public Player {
public:
    Blackj(Game* game) : Player(4, 4, game) {}
    virtual void draw_phase() override;
};

#endif
