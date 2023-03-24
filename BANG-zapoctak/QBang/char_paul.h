#ifndef PAUL_REGRET_H
#define PAUL_REGRET_H

#include "player.h"

//pri zahajeni hry se MU nastavi vzdalenost vsech o jedna VICE

class Paul : public Player {
public:
    Paul(Game* game) : Player(15, 3, "paul", game) {}
};

#endif
