#include "game.h"

bool Lucky::resolve_dyn()
{
    if(index(cards_desk, "Dynamit") == -1)
    {
        return false;
    }

	Card c1 = g->draw_from_deck();
	Card c2 = g->draw_from_deck();
	if (!(c1.rank >= 2 && c1.rank <= 9 && c1.suit == "Piky"))
	{
		g->deck.push_back(c2);
		g->deck.push_back(c1);	

        size_t next = 0;
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(g->game_order[i]->id == id)
            {
                next = (i + 1) % g->player_alive;
            }
        }

        cards_desk[index(cards_desk, "Dynamit")].dyn_active = true;
        g->game_order[next]->cards_desk.push_back(cards_desk[index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + index(cards_desk, "Dynamit"));

		return false;
	}
	else if (!(c2.rank >= 2 && c2.rank <= 9 && c2.suit == "Piky"))
	{
        g->deck.push_back(c1);
        g->deck.push_back(c2);

        size_t next = 0;
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(g->game_order[i]->id == id)
            {
                next = (i + 1) % g->player_alive;
            }
        }

        cards_desk[index(cards_desk, "Dynamit")].dyn_active = true;
        g->game_order[next]->cards_desk.push_back(cards_desk[index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + index(cards_desk, "Dynamit"));

        return false;
	}
	else
	{
		g->deck.push_back(c1);
		g->deck.push_back(c2);

        cards_desk[index(cards_desk, "Dynamit")].dyn_active = false;
        g->deck.push_back(cards_desk[index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + index(cards_desk, "Dynamit"));

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
    g->deck.push_back(cards_desk[index(cards_desk, "Vezeni")]);
    cards_desk.erase(cards_desk.begin() + index(cards_desk, "Vezeni"));
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
