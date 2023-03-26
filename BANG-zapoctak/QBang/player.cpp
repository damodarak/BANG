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
    drawed = true;
}
int Player::game_phase()
{
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        //Dostavnik, WellsFargo, Pivo, Vezeni, Dynamit
        if(cards_hand[i].name == "Dostavnik")
        {
            dostavnik_wells(2);
            return game_phase();
        }
        else if(cards_hand[i].name == "WellsFargo")
        {
            dostavnik_wells(3);
            return game_phase();
        }
        else if(cards_hand[i].name == "Pivo" && health < max_health)
        {
            g->deck.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            health++;
            return game_phase();
        }
        else if(cards_hand[i].name == "Vezeni" && exist_enemy_jail() != -1)
        {
            pass_jail(i, exist_enemy_jail());
            return game_phase();
        }
        else if(cards_hand[i].name == "Dynamit")
        {
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_desk.begin() + i);
            return game_phase();
        }

        //Modre
        else if(cards_hand[i].range > has_gun() && (size_t)best_gun_hand() == i)
        {
            for(size_t j = 0; j < cards_desk.size(); j++)
            {
                if(cards_desk[j].range != 0)
                {
                    discard_card(j + cards_hand.size());
                    cards_desk.push_back(cards_hand[i]);
                    cards_hand.erase(cards_hand.begin() + i);
                    return game_phase();
                }
            }
        }
        else if(cards_hand[i].edge == 'M' && cards_hand[i].range == 0 &&
                index(cards_desk, cards_hand[i].name) == -1)
        {
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return game_phase();
        }

        //neu
        else if(cards_hand[i].card_type == "neu" && play_neu())
        {
           g->deck.push_back(cards_hand[i]);
           cards_hand.erase(cards_hand.begin() + i);
           return 1;
        }

        //agro
        else if(cards_hand[i].card_type == "agr" && cards_hand[i].edge != 'M' && enemies_id.size() != 0)
        {
            string name = cards_hand[i].name;
            if(name == "Duel" || name == "CatBalou")
            {
                target_id = *enemies_id.begin();
                g->deck.push_back(cards_hand[i]);
                cards_hand.erase(cards_hand.begin() + i);
                return 1;
            }
            else if(name == "Bang")
            {
                for(size_t j = 0; j < g->game_order.size(); j++)
                {
                    if(enemies_id.find(g->game_order[j]->id) != enemies_id.end() &&
                        g->distances.find(id)->second[g->game_order[j]->id] <= 1)
                    {
                        target_id = g->game_order[j]->id;
                        g->deck.push_back(cards_hand[i]);
                        cards_hand.erase(cards_hand.begin() + i);
                        played_bang = true;
                        return 1;
                    }
                }
            }
            else if(name == "Panika")
            {
                for(size_t j = 0; j < g->game_order.size(); j++)
                {
                    if(enemies_id.find(g->game_order[j]->id) != enemies_id.end() &&
                        can_play_panika(g->game_order[j]->id))
                    {
                        target_id = g->game_order[j]->id;
                        g->deck.push_back(cards_hand[i]);
                        cards_hand.erase(cards_hand.begin() + i);
                        return 1;
                    }
                }
            }
        }
    }  
    return 0;
}
void Player::dostavnik_wells(int count)
{
    Card c;
    for (size_t i = 0; i < (size_t)count; i++)
    {
        c = g->draw_from_deck();
        cards_hand.push_back(c);
    }
}

int Player::hand_size()
{
    return static_cast<int>(cards_hand.size());
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
bool Player::play_bang()
{
    return true;
}
bool Player::play_vedle()
{
    return true;
}
bool Player::resolve_slab_bang()
{
    return true;
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
void Player::discard_card(size_t index)
{
    if(index >= cards_hand.size())
    {
        size_t i = index - cards_hand.size();
        g->deck.push_back(cards_desk[i]);
        cards_desk.erase(cards_desk.begin() + i);
    }
    else
    {
        g->deck.push_back(cards_hand[index]);
        cards_hand.erase(cards_hand.begin() + index);
    }
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
void Player::set_target_id(const std::string& name)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->name == name)
        {
            target_id = g->game_order[i]->id;
            return;
        }
    }
}
bool Player::has_blue(const std::string &name)
{
    return index(cards_desk, name) != -1;
}
int Player::exist_enemy_jail()
{
    for(auto p : enemies_id)
    {
        if(p != g->game_order[0]->id)
        {
            for(size_t i = 0; i < g->game_order.size(); i++)
            {
                if(g->game_order[i]->id == p && g->game_order[i]->index(g->game_order[i]->cards_desk, "Vezeni") != -1)
                {
                    return p;
                }
            }
        }
    }

    return -1;
}

void Player::pass_jail(int c_index, int id)
{
    Card c;
    c = cards_hand[c_index];
    cards_hand.erase(cards_hand.begin() + c_index);

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            g->game_order[i]->cards_desk.push_back(c);
        }
    }
}

int Player::best_gun_hand()
{
    int best = 0;
    int index = -1;
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].range > best)
        {
            best = cards_hand[i].range;
            index = i;
        }
    }
    return index;
}

bool Player::play_neu()
{
    int s = 0;
    int o = 0;
    int b = 0;
    int v = 0;

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        switch(g->game_order[i]->role)
        {
        case 'S':
            s++;
            break;
        case 'O':
            o++;
            break;
        case 'B':
            b++;
            break;
        case 'V':
            v++;
            break;
        }
    }

    switch(role)
    {
    case 'O':
        return true;
    case 'S':
        return s + v >= o + b;
    case 'V':
        return s + v >= o + b;
    case 'B':
        return s + v <= b;
    default:
        return true;
    }
}

bool Player::can_play_panika(int enemy_id)
{
    int gun = has_gun();
    gun = (gun == -1 ? 0 : gun - 1);
    return g->distances.find(id)->second[enemy_id] + gun <= 1;
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
