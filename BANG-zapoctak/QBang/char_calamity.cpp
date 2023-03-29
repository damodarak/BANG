#include "game.h"

int Calamity::game_phase()
{
    int result = Player::game_phase();
    if(!played_bang && index(cards_hand, "Vedle") != -1 && result == 0)
    {
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(enemies_id.find(g->game_order[i]->id) != enemies_id.end() &&
                g->distances.find(id)->second[g->game_order[i]->id] <= 1)
            {
                target_id = g->game_order[i]->id;
                g->deck.push_back(cards_hand[index(cards_hand, "Vedle")]);
                cards_hand.erase(cards_hand.begin() + index(cards_hand, "Vedle"));
                played_bang = true;
                return 1;
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
        if(index(cards_hand, "Vedle") != -1)
        {
            res = true;
            discard_card(index(cards_hand, "Vedle"));
        }
    }

    return res;
}

bool Calamity::play_vedle()
{
    bool res = Player::play_vedle();

    if(!res)
    {
        if(index(cards_hand, "Bang") != -1)
        {
            res = true;
            discard_card(index(cards_hand, "Bang"));
        }
    }

    return res;
}

bool Calamity::resolve_slab_bang()
{
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
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].name == "Vedle" || cards_hand[i].name == "Bang")
        {
            hand_vedle_bang++;
        }
    }

    if(vedle + hand_vedle_bang >= 2)
    {
        while(vedle < 2)
        {
            if(index(cards_hand, "Vedle") != -1)
            {
                discard_card(index(cards_hand, "Vedle"));
            }
            else
            {
                discard_card(index(cards_hand, "Bang"));
            }
            vedle++;
        }
        return true;
    }
    return false;
}
