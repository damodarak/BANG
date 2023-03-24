#include "game.h"

bool Bart::dec_hp(int lifes)
{
    bool result = Player::dec_hp(lifes);
    if(result)
    {
        Card c;
        for (int i = 0; i < lifes; i++)
        {
            c = g->draw_from_deck();
            cards_hand.push_back(c);
        }
    }
    return result;
}
