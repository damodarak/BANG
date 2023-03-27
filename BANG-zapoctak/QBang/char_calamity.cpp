#include "game.h"

int Calamity::game_phase()
{
    int result = Player::game_phase();
    if(!played_bang && index(cards_hand, "Vedle") != -1)
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
