#include "game.h"
#include "ai.h"

using namespace std;

void Carlson::draw_phase()
{
    Ai::enemy_check(pd);

    vector<Card> v {pd.g->draw_from_deck(), pd.g->draw_from_deck(), pd.g->draw_from_deck()};
    for(int i =0; i < 2; i++)
    {
        int j = choose(v);
        pd.cards_hand.push_back(v[j]);
        v.erase(v.begin() + j);
    }
    pd.g->deck.push_front(v[0]);
    pd.drawed = true;
}
//karty na vyber vlozime do emporia
void Carlson::ability()
{
    if(pd.g->game_order[pd.g->active_player]->id != id)
    {
        return;
    }
    if(pd.drawed)
    {
        pd.ability_used = true;
        return;
    }
    for(int i = 0; i < 3; i++)
    {
        pd.g->emporio.push_back(pd.g->draw_from_deck());
    }
    pd.ability_used = true;
    pd.g->mode = CARLSON_DRAW;
    pd.drawed = true;
}
