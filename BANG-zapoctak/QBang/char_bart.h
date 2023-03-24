#ifndef BART_CASSIDY_H
#define BART_CASSIDY_H

#include "player.h"

//sam si lizne vzdy

class Bart : public Player {
public:
    Bart(Game* game) : Player(12, 4, "bart", game) {}
    virtual bool dec_hp(int lifes) override;
};


#endif
