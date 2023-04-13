#include "game.h"

void Blackj::draw_phase()
{
    //lize dalsi kartu, pokud 2. spluje podminku
    Player::draw_phase();
    if(pd.cards_hand[pd.cards_hand.size() - 1].suit == KARY ||
            pd.cards_hand[pd.cards_hand.size() - 1].suit == SRDCE)
    {
        Card c = pd.g->draw_from_deck();
        pd.cards_hand.push_back(c);
    }
}
