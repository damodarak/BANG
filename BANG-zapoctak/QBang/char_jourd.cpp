#include "game.h"
#include "ai.h"

//zahraje 2x barel ma-li kartu barel, jinak obycyjny barel

bool Jourd::resolve_barrel()
{
    Card c = g->draw_from_deck();
    bool result1 = (c.suit == SRDCE ? true : false);
    g->deck.push_back(c);

    bool result2 = false;
    if(!result1 && Ai::index_name(cards_desk, BAREL) != -1)
    {
        result2 = Player::resolve_barrel();
    }
    return result1 || result2;
}

//for not AI
void Jourd::ability()
{
    if(Ai::no_jourd_abil(g, barel))
    {
        return;
    }

    barel++;
    if(g->mode == KULOMET)
    {
        if(resolve_barrel())
        {
            barel = 0;
            g->neu_turn = (g->neu_turn + 1) % g->player_alive;

            if(g->active_player == g->neu_turn)
            {
                g->mode = NONE;
                g->neu_turn = -1;
            }
        }
    }
    else if(g->mode == SLAB_BANG)
    {
        if(resolve_barrel())
        {   
            played_vedle++;
        }

        if(played_vedle == 2)
        {
            g->mode = NONE;
            barel = 0;
            played_vedle = 0;
        }
    }
    else if(g->mode == VEDLE || g->mode == BANG)
    {
        if(resolve_barrel())
        {
            barel = 0;
            g->mode = NONE;
            played_vedle = 0;
        }
    }
}
