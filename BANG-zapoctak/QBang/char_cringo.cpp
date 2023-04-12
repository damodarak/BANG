#include "game.h"
#include "ai.h"

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
                Ai::discard_card(g, cards_hand, cards_desk, Ai::index_name(cards_hand, PIVO));
                health++;
            }
        }
    }


    //je-li nekym zasazen, tak si od neho vezme kartu z ruky
    if(g->mode == BANG || g->mode == VEDLE || g->mode == SLAB_BANG)
    {
        Ai::cringo_abil(g);
    }



    return health > 0;
}
