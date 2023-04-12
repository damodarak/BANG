#ifndef PAUL_REGRET_H
#define PAUL_REGRET_H

#include "player.h"

//resi set_distance

class Paul : public Player {
public:
    Paul(Game* game) : Player(14, 3, game) {}
};

#endif
