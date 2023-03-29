#include "game.h"

int Slab::game_phase()
{
    int result = Player::game_phase();
    if(g->mode == "Bang")
    {
        g->mode = "Slab";
    }
    return result;
}
