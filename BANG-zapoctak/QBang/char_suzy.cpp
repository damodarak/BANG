#include "game.h"

int Suzy::game_phase()
{
    int result = Player::game_phase();
    ability();
    return result;
}
Card Suzy::give_random_card()
{
    Card c = Player::give_random_card();
    ability();
    return c;
}
Card Suzy::give_random_card_hand()
{
    Card c = Player::give_random_card_hand();
    ability();
    return c;
}
void Suzy::ability()
{
    if(0 == cards_hand.size())
    {
        cards_hand.push_back(g->draw_from_deck());
    }
}
