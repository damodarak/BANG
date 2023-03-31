#include "game.h"

using namespace std;

void Player::draw_phase()
{
    discarded = 0;
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
            g->deck.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }
        else if(cards_hand[i].name == "WellsFargo")
        {
            dostavnik_wells(3);
            g->deck.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }
        else if(cards_hand[i].name == "Pivo" && health < max_health)
        {
            g->deck.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            health++;
            return 2;
        }
        else if(cards_hand[i].name == "Vezeni" && exist_enemy_jail() != -1)
        {
            pass_jail(i, exist_enemy_jail());
            return 2;
        }
        else if(cards_hand[i].name == "Dynamit")
        {
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }

        //Modre
        else if(cards_hand[i].range > has_gun() && best_gun_hand() == static_cast<int>(i))
        {
            //vyhodit starou zbran
            for(size_t j = 0; j < cards_desk.size(); j++)
            {
                if(cards_desk[j].range != 0)
                {
                    discard_card(j + cards_hand.size());               
                }
            }

            //nasadit novou
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            if(cards_desk.back().name == "Volcanic")
            {
                played_bang = false;
            }
            return 2;
        }
        else if(cards_hand[i].edge == 'M' && cards_hand[i].range == 0 &&
                index(cards_desk, cards_hand[i].name) == -1 && cards_hand[i].name != "Vezeni")
        {
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }

        //neu
        else if(cards_hand[i].card_type == "neu" && play_neu(cards_hand[i].name))
        {
           g->deck.push_back(cards_hand[i]);
           cards_hand.erase(cards_hand.begin() + i);
           if(g->deck.back().name == "Salon")
           {
               g->saloon();
               return 2;
           }
           return 1;
        }

        //agro
        else if(cards_hand[i].card_type == "agr" && cards_hand[i].edge != 'M' && enemies_id.size() != 0)
        {
            string name = cards_hand[i].name;
            if(name == "Duel")
            {
                target_id = *enemies_id.begin();
                g->deck.push_back(cards_hand[i]);
                cards_hand.erase(cards_hand.begin() + i);
                g->duel_active_turn = false;
                return 1;
            }
            if(name == "CatBalou")
            {
                for(auto p : enemies_id)
                {
                    if(panika_balou_play(p))
                    {
                        target_id = p;
                        g->deck.push_back(cards_hand[i]);
                        cards_hand.erase(cards_hand.begin() + i);
                        return 1;
                    }
                }
            }
            if(name == "Bang" && !played_bang)
            {
                for(size_t j = 0; j < g->game_order.size(); j++)
                {
                    if(enemies_id.find(g->game_order[j]->id) != enemies_id.end() &&
                        g->distances.find(id)->second[g->game_order[j]->id] <= 1)
                    {
                        target_id = g->game_order[j]->id;
                        g->deck.push_back(cards_hand[i]);
                        cards_hand.erase(cards_hand.begin() + i);
                        played_bang = (index(cards_desk, "Volcanic") != -1 ||
                                name == "willy") ? false : true;
                        return 1;
                    }
                }
            }
            if(name == "Panika")
            {
                for(size_t j = 0; j < g->game_order.size(); j++)
                {
                    if(enemies_id.find(g->game_order[j]->id) != enemies_id.end() &&
                        can_play_panika(g->game_order[j]->id) && panika_balou_play(g->game_order[j]->id))
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
bool Player::panika_balou_play(int enemy_id)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == enemy_id)
        {
            return g->game_order[i]->hand_size() + g->game_order[i]->cards_desk.size() > 0;
        }
    }
    return false;
}
void Player::add_enemy_vice(int enemy_id)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->role == 'V')
        {
            g->game_order[i]->enemies_id.insert(enemy_id);
        }
    }
}
bool Player::has_notai_ability()
{
    if(name == "jourde" || name == "ketchum" || name == "pedro" || name == "jesse" || name == "carlson")
    {
        return true;
    }
    return false;
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
bool Player::dec_hp(int lifes)
{
    health -= lifes;
    int beers = 0;
    if(health <= 0)
    {
        for(size_t i = 0; i < cards_hand.size(); i++)
        {
            if(cards_hand[i].name == "Pivo")
            {
                beers++;
            }
        }
        if(health + beers > 0)
        {
            while(health <= 0)
            {
                discard_card(index(cards_hand, "Pivo"));
                health++;
            }
        }
    }



    return health > 0;
}
bool Player::resolve_jail()
{
    if(index(cards_desk, "Vezeni") == -1)
    {
        return true;
    }

	Card c = g->draw_from_deck();
	bool result = (c.suit == "Srdce" ? true : false);
	g->deck.push_back(c);
    g->deck.push_back(cards_desk[index(cards_desk, "Vezeni")]);
    cards_desk.erase(cards_desk.begin() + index(cards_desk, "Vezeni"));
	return result;
}
bool Player::resolve_dyn()
{
    if(index(cards_desk, "Dynamit") == -1)
    {
        return false;
    }

	Card c = g->draw_from_deck();
	bool result = (c.rank >= 2 && c.rank <= 9 && c.suit == "Piky" ? true : false);
	g->deck.push_back(c);


    size_t next;
    if(!result)//prehodit dalsimu hraci, kdyz nebouchl
    {
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(g->game_order[i]->id == id)
            {
                next = (i + 1) % g->player_alive;
            }
        }

        g->game_order[next]->cards_desk.push_back(cards_desk[index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + index(cards_desk, "Dynamit"));
    }
    else
    {
        g->deck.push_back(cards_desk[index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + index(cards_desk, "Dynamit"));
    }
	return result;
}
bool Player::resolve_barrel()
{
    if(index(cards_desk, "Barel") == -1)
    {
        return false;
    }
    Card c = g->draw_from_deck();
    bool result = (c.suit == "Srdce" ? true : false);
    g->deck.push_back(c);
    return result;
}
bool Player::play_bang()
{
    bool res = false;
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].name == "Bang")
        {
            discard_card(index(cards_hand, "Bang"));
            res = true;
            break;
        }
    }

    if(g->mode == "Duel" && id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            add_enemy_vice(g->game_order[g->active_player]->id);
        }
    }

    return res;
}
bool Player::play_vedle()
{
    bool barel = resolve_barrel();

    if(!barel)
    {
        for(size_t i = 0; i < cards_hand.size(); i++)
        {
            if(cards_hand[i].name == "Vedle")
            {
                discard_card(index(cards_hand, "Vedle"));
                barel = true;
                break;
            }
        }
    }

    if(g->mode == "Bang" || g->mode == "Vedle" || g->mode == "Slab")
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            add_enemy_vice(g->game_order[g->active_player]->id);
        }
    }

    return barel;
}
bool Player::resolve_slab_bang()
{
    bool barel1 = resolve_barrel();
    bool barel2 = resolve_barrel();
    int vedle = 0;

    if(barel1)
    {
        vedle++;
    }
    if(barel2)
    {
        vedle++;
    }

    int hand_vedle = 0;
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].name == "Vedle")
        {
            hand_vedle++;
        }
    }

    enemies_id.insert(g->game_order[g->active_player]->id);
    if(role == 'S')
    {
        add_enemy_vice(g->game_order[g->active_player]->id);
    }

    if(vedle + hand_vedle >= 2)
    {
        while(vedle < 2)
        {
            discard_card(index(cards_hand, "Vedle"));
            vedle++;
        }
        return true;
    }
    return false;
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
    bool end = g->finished();
    switch(role)
    {
        case 'S':
            return ":/cards/cards/sheriff.png";
        case 'V':
            return (health > 0  && isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/deputy.png");
        case 'O':
            return (health > 0  && isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/renegade.png");
        case 'B':
            return (health > 0  && isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/outlaw.png");
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
        if (g->player_count == 5 || g->player_count == 6)
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
    if(id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            add_enemy_vice(g->game_order[g->active_player]->id);
        }
    }

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
        cards_desk.erase(cards_desk.begin() + i - cards_hand.size());
        return c;
    }
}
Card Player::give_random_card_hand()
{
    if(id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            add_enemy_vice(g->game_order[g->active_player]->id);
        }
    }

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
            g->deck.push_back(cards_hand[i]);
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
    if(index(cards, "WellsFargo") != -1)
    {
        return index(cards, "Dostavnik");
    }
    else if(index(cards, "Dostavnik") != -1)
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
    target_id = -1;
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
            int pos = g->id_to_pos(p);
            if(g->game_order[pos]->index(g->game_order[pos]->cards_desk, "Vezeni") == -1)
            {
                return p;
            }
        }
    }

    return -1;
}
void Player::pass_jail(int c_index, int enemy_id)
{
    Card c;
    c = cards_hand[c_index];
    cards_hand.erase(cards_hand.begin() + c_index);

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == enemy_id)
        {
            g->game_order[i]->cards_desk.push_back(c);
            g->game_order[i]->enemies_id.insert(id);
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
bool Player::play_neu(const string& name)
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

    if(name == "Hokynarstvi" || name == "Salon")
    {
        switch(role)
        {
        case 'O':
            return true;
        case 'S':
            return s + v >= b;
        case 'V':
            return s + v >= b;
        case 'B':
            return s + v <= b;
        default:
            return true;
        }
    }
    else
    {
        switch(role)
        {
        case 'O':
            return true;
        case 'S':
            return s + v <= b;
        case 'V':
            return s + v <= b;
        case 'B':
            return s + v + o >= b;
        default:
            return true;
        }
    }

}
bool Player::can_play_panika(int enemy_id)
{
    int gun = has_gun();
    gun = (gun == -1 ? 0 : gun - 1);
    return g->distances.find(id)->second[enemy_id] + gun <= 1;
}
std::vector<Card> Player::give_all_cards()
{
    vector<Card> v;
    while(cards_hand.size() > 0)
    {
        v.push_back(cards_hand[0]);
        cards_hand.erase(cards_hand.begin());
    }
    while(cards_desk.size() > 0)
    {
        v.push_back(cards_desk[0]);
        cards_desk.erase(cards_desk.begin());
    }

    return v;
}
void Player::turn_reset()
{
    played_bang = false;
    played_vedle = 0;
    discarded = 0;
    drawed = false;
    target_id = -1;
    ability_used = false;
}
bool Player::discard_blue()
{
	for (size_t i = 0; i < cards_hand.size(); i++)
	{
		if (cards_hand[i].edge == 'M')
		{
            g->deck.push_back(cards_hand[i]);
			cards_hand.erase(cards_hand.begin() + i);
			return true;
		}
	}
	return false;
}
