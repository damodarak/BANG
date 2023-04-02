#ifndef SLAB_THE_KILLER_H
#define SLAB_THE_KILLER_H

#include "player.h"

//slab zmeni mode pote co zahraje bang

class Slab : public Player {
public:
    Slab(Game* game) : Player(3, 4, "slab", game) {}
};

#endif
