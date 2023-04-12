#include "game.h"

using namespace std;

void Carlson::draw_phase()
{
    //pokud zbyva posledni hrac mimo nas tak ho pridame na seznam nepratel
    if(g->game_order.size() == 2)
    {
        enemies_id.insert(g->game_order[(g->active_player + 1) % 2]->id);
    }
    //Kdyby nahodou si tam pomocnik serifa dal serifa
    if(role == 'V' && enemies_id.find(g->game_order[0]->id) != enemies_id.end())
    {
        enemies_id.erase(enemies_id.find(g->game_order[0]->id));
    }


    vector<Card> v {g->draw_from_deck(), g->draw_from_deck(), g->draw_from_deck()};
    for(int i =0; i < 2; i++)
    {
        int j = choose(v);
        cards_hand.push_back(v[j]);
        v.erase(v.begin() + j);
    }
    g->deck.push_front(v[0]);
    drawed = true;
}
//karty na vyber vlozime do emporia
void Carlson::ability()
{
    if(g->game_order[g->active_player]->id != id)
    {
        return;
    }
    if(drawed)
    {
        ability_used = true;
        return;
    }
    for(int i = 0; i < 3; i++)
    {
        g->emporio.push_back(g->draw_from_deck());
    }
    ability_used = true;
    g->mode = CARLSON_DRAW;
    drawed = true;
}
