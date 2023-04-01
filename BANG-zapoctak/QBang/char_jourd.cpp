#include "game.h"

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
    if(g->mode == "Kulomet")
    {
        barel++;
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
        barel++;
        if(resolve_barrel())
        {
            barel++;
            played_vedle++;
        }
        else
        {
            barel++;
        }


        if(played_vedle == 2)
        {
            g->mode = "";
        }
    }
    else if(g->mode == "Vedle" || g->mode == "Bang")
    {
        barel++;
        if(resolve_barrel())
        {
            barel = 0;
            g->mode = "";
        }
        else
        {
            barel++;
        }


        if(played_vedle == 1)
        {
            g->mode = "";
        }

        if(barel == 2)
        {
            ability_used = true;
        }

    }
}
