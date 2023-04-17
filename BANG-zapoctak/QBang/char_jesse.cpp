#include "game.h"
#include "ai.h"

void Jesse::draw_phase()
{
    Ai::enemy_check(pd);

    if(pd.isai)
    {
        if(!Ai::jesse_abil(pd.g))
        {
            Player::draw_phase();
        }
    }
    else
    {
        Player::draw_phase();
    }
}

void Jesse::ability()
{
    if(pd.isai)
    {
        return;
    }
    if(target_id == -1 || GameTools::hand_size(pd.g, target_id) == 0)
    {
        return;
    }
    pd.ability_used = true;
    pd.drawed = true;
    pd.cards_hand.push_back(pd.g->game_order[GameTools::id_to_pos(pd.g, target_id)]->give_random_card_hand());
    pd.cards_hand.push_back(pd.g->draw_from_deck());
}
