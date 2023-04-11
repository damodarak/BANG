#include "game.h"
#include "ai.h"

void Jesse::draw_phase()
{
    //pokud zbyva posledni hrac mimo nas tak ho pridame na seznam nepratel
    if(g->game_order.size() == 2)
    {
        enemies_id.insert(g->game_order[(g->active_player + 1) % 2]->id);
    }
    //Kdyby nahodou si tam pomocnik serifa dal serifa
    if(role == 'V' && enemies_id.find(g->game_order[0]->id) != enemies_id.end())
    {
        enemies_id.erase(enemies_id.find(g->game_order[0]->id));
    }

    if(g->game_order[g->active_player]->isai)
    {
        if(!Ai::jesse_abil(g))
        {
            Player::draw_phase();
        }
    }
}

void Jesse::ability()
{
    if(g->game_order[g->active_player]->isai)
    {
        return;
    }
    if(target_id == -1 || GameTools::hand_size(g, target_id) == 0)
    {
        return;
    }
    ability_used = true;
    drawed = true;
    cards_hand.push_back(g->game_order[GameTools::id_to_pos(g, target_id)]->give_random_card_hand());
    cards_hand.push_back(g->draw_from_deck());
}

