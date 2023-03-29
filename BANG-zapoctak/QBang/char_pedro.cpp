#include "game.h"

void Pedro::draw_phase()
{
    ability_used = true;
    drawed = true;

    if(g->deck.back().name == "Dostavnik" || g->deck.back().name == "WellsFargo")
    {
        cards_hand.push_back(g->draw_from_deck());
        cards_hand.push_back(g->deck.back());
        g->deck.pop_back();
        return;
    }
    else if((health > max_health / 2 && g->deck.back().card_type == "agr") ||
            (health <= max_health / 2 && g->deck.back().card_type == "def"))
    {
        cards_hand.push_back(g->draw_from_deck());
        cards_hand.push_back(g->deck.back());
        g->deck.pop_back();
        return;
    }
    else
    {
        Player::draw_phase();
    }
}

void Pedro::ability()
{
    if(ability_used)
    {
        return;
    }
    cards_hand.push_back(g->draw_from_deck());
    cards_hand.push_back(g->deck.back());
    g->deck.pop_back();
    ability_used = true;
    drawed = true;
}
