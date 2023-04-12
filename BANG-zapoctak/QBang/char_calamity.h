#ifndef CALAMITY_JANET_H
#define CALAMITY_JANET_H

#include "player.h"

//v play_bang/vedle(), resolve_slab_bang() muze pouzit opacne karty nez je treba, pokud nema ty spravne
//game_phase zahraje normalne a pokud nehrala bang a ma vedle tak po nekom vystreli

class Calamity : public Player {
public:
    Calamity(Game* game) : Player(1, 4, game) {}
    virtual int game_phase() override;
    virtual bool play_bang() override;
    virtual bool play_vedle() override;
    virtual bool resolve_slab_bang() override;
};

#endif
