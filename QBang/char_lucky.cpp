#include "game.h"
#include "ai.h"

//vsechny tyto overridy si liznou 2 karty

bool Lucky::resolve_dyn()
{
    if(Ai::index_name(cards_desk, DYNAMIT) == -1)
    {
        return false;
    }

    Card c1 = pd.g->draw_from_deck();
    Card c2 = pd.g->draw_from_deck();
    if (!(c1.rank >= 2 && c1.rank <= 9 && c1.suit == PIKY))
	{
        pd.g->deck.push_back(c2);
        pd.g->deck.push_back(c1);

        size_t next = (pd.g->active_player + 1) % pd.g->player_alive;

        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = true;
        pd.g->game_order[next]->cards_desk.push_back(cards_desk[Ai::index_name(cards_desk, DYNAMIT)]);
        cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, DYNAMIT));

		return false;
	}
    else if (!(c2.rank >= 2 && c2.rank <= 9 && c2.suit == PIKY))
	{
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);

        size_t next = (pd.g->active_player + 1) % pd.g->player_alive;

        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = true;
        pd.g->game_order[next]->cards_desk.push_back(cards_desk[Ai::index_name(cards_desk, DYNAMIT)]);
        cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, DYNAMIT));

        return false;
	}
	else
	{
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);

        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = false;
        pd.g->deck.push_back(cards_desk[Ai::index_name(cards_desk, DYNAMIT)]);
        cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, DYNAMIT));

		return true;
	}
}
bool Lucky::resolve_jail()
{
    if(Ai::index_name(cards_desk, VEZENI) == -1)
    {
        return true;
    }

    Card c1 = pd.g->draw_from_deck();
    Card c2 = pd.g->draw_from_deck();
    pd.g->deck.push_back(cards_desk[Ai::index_name(cards_desk, VEZENI)]);
    cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, VEZENI));
    if (c1.suit == SRDCE)
	{
        pd.g->deck.push_back(c2);
        pd.g->deck.push_back(c1);
		return true;
	}
    else if (c2.suit == SRDCE)
	{
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);
		return true;
	}
	else
	{
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);
		return false;
	}
}
bool Lucky::resolve_barrel()
{
    if(Ai::index_name(cards_desk, BAREL) == -1)
    {
        return false;
    }

    Card c1 = pd.g->draw_from_deck();
    Card c2 = pd.g->draw_from_deck();
    if (c1.suit == SRDCE)
    {
        pd.g->deck.push_back(c2);
        pd.g->deck.push_back(c1);
        return true;
    }
    else if (c2.suit == SRDCE)
    {
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);
        return true;
    }
    else
    {
        pd.g->deck.push_back(c1);
        pd.g->deck.push_back(c2);
        return false;
    }
}
