#ifndef SLAB_THE_KILLER_H
#define SLAB_THE_KILLER_H

#include "player.h"

//slab bude sam davat miss2 do actionu nebo tak neco nebo hra

class Slab : public Player {
public:
    Slab(Game* game) : Player(3, 4, "slab", game) {}
    virtual int game_phase() override;
};

#endif
