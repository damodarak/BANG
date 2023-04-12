#ifndef BART_CASSIDY_H
#define BART_CASSIDY_H

#include "player.h"

//sam si lizne vzdy, kdykoli je zasazen a jsou mu ubrany zivoty

class Bart : public Player {
public:
    Bart(Game* game) : Player(11, 4, game) {}
    virtual bool dec_hp(int lifes) override;
};

#endif
