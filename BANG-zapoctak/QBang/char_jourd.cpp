#include "game.h"

bool Jourd::resolve_barrel()
{
    bool result1 = Player::resolve_barrel();
    bool result2 = false;
    if(index(cards_desk, "Barel") != -1)
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
