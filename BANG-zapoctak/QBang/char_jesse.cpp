#include "game.h"
#include "ai.h"

void Jesse::draw_phase()
{
    //pokud zbyva posledni hrac mimo nas tak ho pridame na seznam nepratel
    if(pd.g->game_order.size() == 2)
    {
        pd.enemies_id.insert(pd.g->game_order[(pd.g->active_player + 1) % 2]->id);
    }
    //Kdyby nahodou si tam pomocnik serifa dal serifa
    if(pd.role == 'V' && pd.enemies_id.find(pd.g->game_order[0]->id) != pd.enemies_id.end())
    {
        pd.enemies_id.erase(pd.enemies_id.find(pd.g->game_order[0]->id));
    }

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

