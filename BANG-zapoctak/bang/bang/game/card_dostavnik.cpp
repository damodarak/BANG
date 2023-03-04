#include "card_dostavnik.h"

using namespace std;

vector<Card> Dostavnik::Play(Game* g)
{
	vector<Card> cards;
	for (size_t i = 0; i < cards_to_draw; i++)
	{
		Card c = g->draw_from_deck();
		cards.push_back(c);
	}
	return cards;
}