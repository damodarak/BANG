#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "card.h"
#include <set>

class Game;

class PlayerData {
public:
    PlayerData(int rank, Game* game) : isai(true), ranking(rank), discarded(0),
        played_bang(false), drawed(false), ability_used(false), barel(0), played_vedle(0),
        role('?'), g(game) {}

    bool isai;
    int ranking;//for AI to choose beter character
    int discarded;
    bool played_bang;//po kazdem bangu se rovnou podivame, jestli mame Volcanic a pripadne zmenime tuto hodnotu
    bool drawed;
    bool ability_used;
    int barel;
    int played_vedle;

    char role;
    std::vector<Card> cards_hand;
    std::set<int> enemies_id;
    Game* g;
};

#endif
