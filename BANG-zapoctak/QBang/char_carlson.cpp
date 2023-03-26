#include "game.h"

using namespace std;

void Carlson::draw_phase()
{
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
void Carlson::ability()
{
    //dodelat pro notai hrace
}
