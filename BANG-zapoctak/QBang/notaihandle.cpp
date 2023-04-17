#include "notaihandle.h"
#include "game.h"
#include "ai.h"

void NotaiHandle::resolve_notai_play(Game *g)
{
    int enemy_pos = GameTools::id_to_pos(g, g->game_order[g->active_player]->target_id);

    //zbran
    if(g->deck.back().range != 0)
    {
        gun(g);
    }
    //vezeni
    else if(g->deck.back().mode == VEZENI)
    {
        jail(g, enemy_pos);
    }
    //ostatni modre karty
    else if(g->deck.back().edge == 'M')
    {
        blue(g);
    }
    //karty s hnedym okrajem
    else
    {
        g->deck.back().play(g->active_player, g->game_order[g->active_player]->data());
    }

    g->set_distances();
}

void NotaiHandle::gun(Game *g)
{
    if(Ai::has_gun(g->game_order[g->active_player]->cards_desk) == -1)
    {
        Card c = g->deck.back();
        g->deck.pop_back();
        g->game_order[g->active_player]->cards_desk.push_back(c);
        if(c.mode == VOLCANIC)
        {
            g->game_order[g->active_player]->data().played_bang = false;
        }
    }
}

void NotaiHandle::jail(Game *g, int enemy_pos)
{
    if(enemy_pos != -1 && g->game_order[enemy_pos]->data().role != 'S'
        && Ai::index_name(g->game_order[enemy_pos]->cards_desk, VEZENI) == -1)
    {
        Card c = g->deck.back();
        g->deck.pop_back();
        g->game_order[enemy_pos]->cards_desk.push_back(c);
        g->game_order[enemy_pos]->data().enemies_id.insert(g->game_order[g->active_player]->id);
    }
}

void NotaiHandle::blue(Game *g)
{
    if(Ai::index_name(g->game_order[g->active_player]->cards_desk, g->deck.back().mode) == -1)
    {
        Card c = g->deck.back();
        g->deck.pop_back();
        g->game_order[g->active_player]->cards_desk.push_back(c);
    }
}
