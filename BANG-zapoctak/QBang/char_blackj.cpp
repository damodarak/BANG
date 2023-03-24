#include "game.h"

void Blackj::draw_phase()
{
    Player::draw_phase();
    if(cards_hand[cards_hand.size()-1].suit == "Kary" || cards_hand[cards_hand.size()-1].suit == "Srdce")
    {
        Card c = g->draw_from_deck();
        cards_hand.push_back(c);
    }
}
