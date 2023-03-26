#include "game.h"

void Jesse::draw_phase()
{
    int index = -1;
    int cards = -1;
    if(enemies_id.size() != 0)
    {
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(enemies_id.find(g->game_order[i]->id) != enemies_id.end() &&
                    g->game_order[i]->hand_size() > cards)
            {
                cards = g->game_order[i]->hand_size();
                index = i;
            }
        }
    }

    if(cards == -1)
    {
        Player::draw_phase();
    }
    else
    {
        ability_used = true;
        cards_hand.push_back(g->game_order[index]->give_random_card_hand());
        cards_hand.push_back(g->draw_from_deck());
    }
}

void Jesse::ability()
{
    if(ability_used)
    {
        return;
    }
    ability_used = true;

}
