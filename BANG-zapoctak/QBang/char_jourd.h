#ifndef JOURDONNAIS_H
#define JOURDONNAIS_H

#include "player.h"

//funkce vedle, nebo skryty barel, jina karta nez obyc barel
//resolve slab bang resi i tuto postavu

class Jourd : public Player {
public:
    Jourd(Game* game) : Player(3, 4, game) {}
    virtual bool resolve_barrel() override;
    virtual void ability() override;
};

#endif
