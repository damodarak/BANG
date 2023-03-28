#include "game.h"

bool Jourd::resolve_barrel()
{
    Card c = g->draw_from_deck();
    bool result1 = (c.suit == "Srdce" ? true : false);
    g->deck.push_back(c);

    bool result2 = false;
    if(!result1 && index(cards_desk, "Barel") != -1)
    {
        result2 = Player::resolve_barrel();
    }
    return result1 || result2;
}

void Jourd::ability()
{
    bool result = resolve_barrel();
    played_vedle += (result ? 1 : 0);
}
