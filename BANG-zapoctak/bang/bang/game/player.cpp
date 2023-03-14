#include "game.h"

using namespace std;

void Player::draw_phase()
{
	Card c;
	for (size_t i = 0; i < 2; i++)
	{
		c = g->draw_from_deck();
		cards_hand.push_back(c);
	}
}
void Player::discard_phase()
{
	/*
	* > max_health/2 => odhazuju nejdriv 'neu' karty pak 'def' pak 'M' pak 'agr'
	* <= max_health/2 => odhazuju nejriv 'neu' pak 'M' pak 'agr' pak 'def'
	*/
	if (cards_hand.size() <= health)
	{
		return;
	}

	while (cards_hand.size() > health)
	{
		if (health > max_healt / 2)
		{
			bool result = (discard_card("neu") ? true : false);
			result = (result ? true : discard_card("def"));
			result = (result ? true : discard_blue());
			result = (result ? true : discard_card("agr"));
		}
		else
		{
			bool result = (discard_card("neu") ? true : false);
			result = (result ? true : discard_blue());
			result = (result ? true : discard_card("agr"));
			result = (result ? true : discard_card("def"));
		}
	}
}
bool Player::resolve_jail()
{
	Card c = g->draw_from_deck();
	bool result = (c.suit == "Srdce" ? true : false);
	g->deck.push_back(c);
	return result;
}
bool Player::resolve_dyn()
{
	Card c = g->draw_from_deck();
	bool result = (c.rank >= 2 && c.rank <= 9 && c.suit == "Piky" ? true : false);
	g->deck.push_back(c);
	return result;
}
bool Player::resolve_barrel()
{
	return resolve_jail();
}
void Player::set_role(char r)
{
	role = r;
	if (role == 'S')
	{
		max_healt++;
		health++;
	}
}
char Player::say_role()
{
	return (role == 'S' ? 'S' : '?');
}
void Player::take_card(Card& c)
{
	cards_hand.push_back(c);
}
void Player::set_enemy(int sheriff, const vector<int>& ids)
{
	switch (role)
	{
	case 'O':
		for (auto&& pl : ids)
		{
			if (pl != sheriff && pl != id)
			{
				enemies_id.insert(pl);
			}
		}
		break;
	case 'B':
		enemies_id.insert(sheriff);
		break;
	case 'V':
		if (g->player_count == 5)
		{
			for (auto&& pl : ids)
			{
				if (pl != sheriff && pl != id)
				{
					enemies_id.insert(pl);
				}
			}
		}
		break;
	default:
		break;
	}
}
bool Player::discard_card(const string& type)
{
	for (size_t i = 0; i < cards_hand.size(); i++)
	{
		if (cards_hand[i].card_type == type)
		{
			g->deck.push_back(cards_desk[i]);
			cards_hand.erase(cards_hand.begin() + i);
			return true;
		}
	}
	return false;
}
bool Player::discard_blue()
{
	for (size_t i = 0; i < cards_hand.size(); i++)
	{
		if (cards_hand[i].edge == 'M')
		{
			g->deck.push_back(cards_desk[i]);
			cards_hand.erase(cards_hand.begin() + i);
			return true;
		}
	}
	return false;
}