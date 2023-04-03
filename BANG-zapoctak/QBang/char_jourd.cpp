#include "game.h"

//zahraje 2x barel ma-li kartu barel, jinak obycyjny barel

bool Jourd::resolve_barrel()
{
    Card c = g->draw_from_deck();
    bool result1 = (c.suit == "Srdce" ? true : false);
    g->deck.push_back(c);

    bool result2 = false;
    if(!result1 && index(cards_desk, "Barel") != -1)
    {
        result2 = Player::resolve_barrel();
    }
    return result1 || result2;
}

//for not AI
void Jourd::ability()
{
    if((g->game_order[g->active_player]->name == "slab" && barel == 2) ||
            (g->game_order[g->active_player]->name != "slab" && barel == 1))
    {
        return;
    }


    barel++;
    if(g->mode == "Kulomet")
    {
        if(resolve_barrel())
        {
            barel = 0;
            g->neu_turn = (g->neu_turn + 1) % g->player_alive;

            if(g->active_player == g->neu_turn)
            {
                g->mode = "";
                g->neu_turn = -1;
            }
        }
    }
    else if(g->mode == "Slab")
    {
        if(resolve_barrel())
        {   
            played_vedle++;
        }

        if(played_vedle == 2)
        {
            g->mode = "";
            barel = 0;
            played_vedle = 0;
        }
    }
    else if(g->mode == "Vedle" || g->mode == "Bang")
    {
        if(resolve_barrel())
        {
            barel = 0;
            g->mode = "";
            played_vedle = 0;
        }
    }
}
