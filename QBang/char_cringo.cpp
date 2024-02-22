#include "game.h"
#include "ai.h"

bool Cringo::dec_hp(int lifes)
{
    health -= lifes;
    int beers = 0;
    if(health <= 0)
    {
        for(size_t i = 0; i < pd.cards_hand.size(); i++)
        {
            if(pd.cards_hand[i].mode == PIVO)
            {
                beers++;
            }
        }
        if(health + beers > 0)
        {
            while(health <= 0)
            {
                Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, PIVO));
                health++;
            }
        }
    }

    //je-li nekym zasazen, tak si od neho vezme kartu z ruky
    if(pd.g->mode == BANG || pd.g->mode == VEDLE || pd.g->mode == SLAB_BANG)
    {
        Ai::cringo_abil(pd.g);
    }

    return health > 0;
}
