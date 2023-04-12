#ifndef ROSE_DOOLAN_H
#define ROSE_DOOLAN_H

#include "player.h"

//resi set_distance()

class Rose : public Player {
public:
    Rose(Game* game) : Player(12, 4, game) {}
};

#endif
