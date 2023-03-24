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
    if (cards_hand.size() <= (size_t)health)
	{
		return;
	}

    while (cards_hand.size() > (size_t)health)
	{
        if (health > max_health / 2)
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
    bool result = resolve_jail();
    played_vedle += (result ? 1 : 0);
    return result;
}
int Player::card_count()
{
    return cards_hand.size();
}
QString Player::file_loc()
{
    string end = ":/char_img/char_img/" + name + ".png";
    return QString::fromStdString(end);
}
QString Player::role_loc()
{
    switch(role)
    {
        case 'S':
            return ":/cards/cards/sheriff.png";
        case 'V':
            return (health > 0  && isai ? ":/cards/cards/back-role.png" : ":/cards/cards/deputy.png");
        case 'O':
            return (health > 0  && isai ? ":/cards/cards/back-role.png" : ":/cards/cards/renegade.png");
        case 'B':
            return (health > 0  && isai ? ":/cards/cards/back-role.png" : ":/cards/cards/outlaw.png");
        default:
            break;
    }
    return "";
}

int Player::has_gun()
{
    for(size_t i = 0; i < cards_desk.size(); i++)
    {
        if(cards_desk[i].range != 0)
        {
            return cards_desk[i].range;
        }
    }
    return -1;
}
void Player::set_role(char r)
{
	role = r;
	if (role == 'S')
	{
        max_health++;
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
Card Player::give_random_card()
{
    size_t max = cards_hand.size() + cards_desk.size() - 1;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,max);

    size_t i = dist(rng);
    if(i < cards_hand.size())
    {
        Card c;
        c = cards_hand[i];
        cards_hand.erase(cards_hand.begin() + i);
        return c;
    }
    else
    {
        Card c;
        c = cards_desk[i - cards_hand.size()];
        cards_desk.erase(cards_hand.begin() + i - cards_hand.size());
        return c;
    }
}
Card Player::give_random_card_hand()
{
    size_t max = cards_hand.size() - 1;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,max);

    size_t i = dist(rng);
    Card c;
    c = cards_hand[i];
    cards_hand.erase(cards_hand.begin() + i);
    return c;
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
void Player::discard_card(int index)
{
    g->deck.push_back(cards_hand[index]);
    cards_hand.erase(cards_hand.begin() + index);
}
int Player::index(const std::vector<Card>& cards, const std::string& name_type)
{
    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].name == name_type || cards[i].card_type == name_type)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}
int Player::choose(const std::vector<Card>& cards)
{
    if(index(cards, "Dostavnik") != -1)
    {
        return index(cards, "Dostavnik");
    }
    else if(index(cards, "WellsFargo") != -1)
    {
        return index(cards, "WellsFargo");
    }
    else if(health > max_health / 2)
    {
        if(index(cards, "agr") != -1)
        {
            return index(cards, "agr");
        }
        else if(index(cards, "def") != -1)
        {
            return index(cards, "def");
        }
        else
        {
            return index(cards, "neu");
        }
    }
    else
    {
        if(index(cards, "def") != -1)
        {
            return index(cards, "def");
        }
        else if(index(cards, "agr") != -1)
        {
            return index(cards, "agr");
        }
        else
        {
            return index(cards, "neu");
        }
    }
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
