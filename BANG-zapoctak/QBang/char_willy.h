#ifndef WILLY_THE_KID_H
#define WILLY_THE_KID_H

#include "player.h"

//vyreseno v player.cpp game_phase()
//taky to resi game_loop()

class Willy : public Player {
public:
    Willy(Game* game) : Player(1, 4, "willy", game) {}
};

#endif
