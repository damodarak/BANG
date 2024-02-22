#include "game.h"
#include "ai.h"

//zahraje 2x barel ma-li kartu barel, jinak obycyjny barel

bool Jourd::resolve_barrel()
{
    Card c = pd.g->draw_from_deck();
    bool result1 = c.suit == SRDCE;
    pd.g->deck.push_back(c);

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
    if(Ai::no_jourd_abil(pd.g, pd.barel))
    {
        return;
    }

    pd.barel++;
    if(pd.g->mode == KULOMET)
    {
        if(resolve_barrel())
        {
            pd.barel = 0;
            pd.g->neu_turn = (pd.g->neu_turn + 1) % pd.g->player_alive;

            if(pd.g->active_player == pd.g->neu_turn)
            {
                pd.g->mode = NONE;
                pd.g->neu_turn = -1;
            }
        }
    }
    else if(pd.g->mode == SLAB_BANG)
    {
        if(resolve_barrel())
        {   
            pd.played_vedle++;
        }

        if(pd.played_vedle == 2)
        {
            pd.g->mode = NONE;
            pd.barel = 0;
            pd.played_vedle = 0;
        }
    }
    else if(pd.g->mode == VEDLE || pd.g->mode == BANG)
    {
        if(resolve_barrel())
        {
            pd.barel = 0;
            pd.g->mode = NONE;
            pd.played_vedle = 0;
        }
    }
}
