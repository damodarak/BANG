#include "game.h"

void Blackj::draw_phase()
{
    //lize dalsi kartu, pokud 2. spluje podminku
    Player::draw_phase();
    if(cards_hand[cards_hand.size() - 1].suit == KARY ||
            cards_hand[cards_hand.size() - 1].suit == SRDCE)
    {
        Card c = g->draw_from_deck();
        cards_hand.push_back(c);
    }
}
