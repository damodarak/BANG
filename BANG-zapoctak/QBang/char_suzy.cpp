#include "game.h"

//override metody zavolaji puvodni metody a nasledne zavolaji schopnost Suzy

bool Suzy::game_phase()
{
    bool result = Player::game_phase();
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

bool Suzy::play_bang()
{
    bool res = Player::play_bang();
    ability();
    return res;
}

bool Suzy::play_vedle()
{
    bool res = Player::play_vedle();
    ability();
    return res;
}

bool Suzy::resolve_slab_bang()
{
    bool res = Player::resolve_slab_bang();
    ability();
    return res;
}
void Suzy::ability()
{
    if(0 == pd.cards_hand.size())
    {
        pd.cards_hand.push_back(pd.g->draw_from_deck());
    }
}

std::vector<Card> Suzy::give_all_cards()
{
    if(health != 0)
    {
        std::vector<Card> v = Player::give_all_cards();
        ability();
        return v;
    }
    else
    {
        return Player::give_all_cards();
    }
}
