#ifndef SLAB_THE_KILLER_H
#define SLAB_THE_KILLER_H

#include "player.h"

//slab zmeni mode pote co zahraje bang na "Slab", specialni funkce pro handling

class Slab : public Player {
public:
    Slab(Game* game) : Player(2, 4, game) {}
};

#endif
