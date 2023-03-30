#include "game.h"

bool Cringo::dec_hp(int lifes)
{
    health -= lifes;
    int beers = 0;
    if(health <= 0)
    {
        for(size_t i = 0; i < cards_hand.size(); i++)
        {
            if(cards_hand[i].name == "Pivo")
            {
                beers++;
            }
        }
        if(health + beers > 0)
        {
            while(health <= 0)
            {
                discard_card(index(cards_hand, "Pivo"));
                health++;
            }
        }
    }

    if(g->mode == "Bang" || g->mode == "Vedle" || g->mode == "Vedle" || g->mode == "Slab")
    {
        cards_hand.push_back(g->game_order[g->active_player]->give_random_card_hand());
    }



    return health > 0;
}
