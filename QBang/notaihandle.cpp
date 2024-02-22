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

void NotaiHandle::resolve_notai_react(Game *g, size_t c_index)
{
    Card c;
    //barel
    if(c_index >= g->game_order[g->notai]->data().cards_hand.size())
    {
        c = g->game_order[g->notai]->cards_desk[Ai::index_name(g->game_order[g->notai]->cards_desk, BAREL)];
    }
    else
    {
        c = g->game_order[g->notai]->data().cards_hand[c_index];
        if((Modes)c.mode == BAREL)
        {
            return;
        }
        g->game_order[g->notai]->data().cards_hand.erase(g->game_order[g->notai]->data().cards_hand.begin() + c_index);
        g->deck.push_back(c);
    }

    switch(g->mode)
    {
    case DUEL:
        g->duel_active_turn = !g->duel_active_turn;
        break;
    case INDIANI:
        g->next_neu();
        break;
    case KULOMET:
        notai_react_vedle(g, c.mode);
        break;
    case BANG:
        notai_react_vedle(g, c.mode);
        break;
    case VEDLE:
        notai_react_vedle(g, c.mode);
        break;
    case SLAB_BANG:
        notai_react_slab(g, c.mode);
        break;
    default:
        break;
    }
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

void NotaiHandle::notai_react_vedle(Game *g, int card_mode)
{
    if((Modes)card_mode == BAREL)
    {
        if(g->game_order[g->notai]->resolve_barrel())
        {
            g->game_order[g->notai]->data().barel = 0;
            g->next_neu();
        }
        else
        {
            g->game_order[g->notai]->data().barel++;
        }
    }
    else
    {
        g->next_neu();
    }
}

void NotaiHandle::notai_react_slab(Game *g, int card_mode)
{
    if((Modes)card_mode == BAREL)
    {
        if(g->game_order[g->notai]->resolve_barrel())
        {
            g->game_order[g->notai]->data().barel++;
            g->game_order[g->notai]->data().played_vedle++;
        }
        else
        {
            g->game_order[g->notai]->data().barel++;
        }
    }
    else
    {
        g->game_order[g->notai]->data().played_vedle++;
    }

    if(g->game_order[g->notai]->data().played_vedle == 2)
    {
        g->mode = NONE;
        g->game_order[g->notai]->data().barel = 0;
        g->game_order[g->notai]->data().played_vedle = 0;
    }
}
