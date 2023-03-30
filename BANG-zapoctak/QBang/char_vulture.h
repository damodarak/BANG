#ifndef VULTURE_SAM_H
#define VULTURE_SAM_H

#include "player.h"

//resi hra

class Vulture : public Player {
public:
    Vulture(Game* game) : Player(9, 4, "vulture", game) {}
};

#endif
