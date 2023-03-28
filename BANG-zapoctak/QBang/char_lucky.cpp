#include "game.h"

bool Lucky::resolve_dyn()
{
    if(index(cards_desk, "Dynamite") == -1)
    {
        return false;
    }

	Card c1 = g->draw_from_deck();
	Card c2 = g->draw_from_deck();
	if (!(c1.rank >= 2 && c1.rank <= 9 && c1.suit == "Piky"))
	{
		g->deck.push_back(c2);
		g->deck.push_back(c1);	
		return false;
	}
	else if (!(c2.rank >= 2 && c2.rank <= 9 && c2.suit == "Piky"))
	{
		g->deck.push_back(c1);
		g->deck.push_back(c2);
		return false;
	}
	else
	{
		g->deck.push_back(c1);
		g->deck.push_back(c2);
		return true;
	}
}
bool Lucky::resolve_jail()
{
    if(index(cards_desk, "Vezeni") == -1)
    {
        return true;
    }


	Card c1 = g->draw_from_deck();
	Card c2 = g->draw_from_deck();
	if (c1.suit == "Srdce")
	{
		g->deck.push_back(c2);
		g->deck.push_back(c1);	
		return true;
	}
	else if (c2.suit == "Srdce")
	{
		g->deck.push_back(c1);
		g->deck.push_back(c2);	
		return true;
	}
	else
	{
		g->deck.push_back(c1);
		g->deck.push_back(c2);
		return false;
	}
}
bool Lucky::resolve_barrel()
{
    if(index(cards_desk, "Barel") == -1)
    {
        return false;
    }


    Card c1 = g->draw_from_deck();
    Card c2 = g->draw_from_deck();
    if (c1.suit == "Srdce")
    {
        g->deck.push_back(c2);
        g->deck.push_back(c1);
        return true;
    }
    else if (c2.suit == "Srdce")
    {
        g->deck.push_back(c1);
        g->deck.push_back(c2);
        return true;
    }
    else
    {
        g->deck.push_back(c1);
        g->deck.push_back(c2);
        return false;
    }
}
