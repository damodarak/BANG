#include "game.h"

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


    int index = -1;
    int cards = -1;
    //najde mezi neprateli nejvetsi pocet karet v ruce a tam odebere kartu
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

    if(cards == -1 || cards == 0)
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
    if(g->game_order[g->active_player]->isai)
    {
        return;
    }
    if(target_id == -1 || g->game_order[g->id_to_pos(target_id)]->hand_size() == 0)
    {
        return;
    }
    ability_used = true;
    drawed = true;
    cards_hand.push_back(g->game_order[g->id_to_pos(target_id)]->give_random_card_hand());
    cards_hand.push_back(g->draw_from_deck());
}
