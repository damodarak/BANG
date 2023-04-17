#include "game.h"
#include "ai.h"

bool Calamity::game_phase()
{
    bool result = Player::game_phase();
    if(!pd.played_bang && Ai::index_name(pd.cards_hand, VEDLE) != -1 && result == 0)
    {
        for(size_t i = 0; i < pd.g->game_order.size(); i++)
        {
            if(pd.enemies_id.find(pd.g->game_order[i]->id) != pd.enemies_id.end() &&
                pd.g->distances.find(id)->second[pd.g->game_order[i]->id] <= 1)
            {
                target_id = pd.g->game_order[i]->id;
                pd.g->deck.push_back(pd.cards_hand[Ai::index_name(pd.cards_hand, VEDLE)]);
                pd.cards_hand.erase(pd.cards_hand.begin() + Ai::index_name(pd.cards_hand, VEDLE));
                pd.played_bang = true;
                return true;
            }
        }
    }
    return result;
}

bool Calamity::play_bang()
{
    bool res = Player::play_bang();

    if(!res)
    {
        if(Ai::index_name(pd.cards_hand, VEDLE) != -1)
        {
            res = true;
            Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, VEDLE));
        }
    }
    return res;
}

bool Calamity::play_vedle()
{
    bool res = Player::play_vedle();

    if(!res)
    {
        if(Ai::index_name(pd.cards_hand, BANG) != -1)
        {
            res = true;
            Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, BANG));
        }
    }
    return res;
}

bool Calamity::resolve_slab_bang()
{
    add_enemy();

    bool barel1 = resolve_barrel();
    bool barel2 = resolve_barrel();
    int vedle = 0;

    if(barel1)
    {
        vedle++;
    }
    if(barel2)
    {
        vedle++;
    }

    int hand_vedle_bang = 0;
    for(size_t i = 0; i < pd.cards_hand.size(); i++)
    {
        if(pd.cards_hand[i].mode == VEDLE || pd.cards_hand[i].mode == BANG)
        {
            hand_vedle_bang++;
        }
    }

    //potrebujeme odhodit 2x vedle
    if(vedle + hand_vedle_bang >= 2)
    {
        while(vedle < 2)
        {
            if(Ai::index_name(pd.cards_hand, VEDLE) != -1)
            {
                Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, VEDLE));
            }
            else
            {
                Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, BANG));
            }
            vedle++;
        }
        return true;
    }
    return false;
}
