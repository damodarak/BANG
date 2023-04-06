#include "game.h"
#include "ai.h"

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
    if(role == 'O' && g->game_order.size() != 2 && enemies_id.find(g->game_order[0]->id) != enemies_id.end())
    {
        enemies_id.erase(enemies_id.find(g->game_order[0]->id));
    }


	Card c;
    //ze zacatku se lize 2 karty
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
            //musi existovat nekdo komu muzeme predat vezeni a je nas nepritel, nesmime to dat serifovi
            pass_jail(i, exist_enemy_jail());
            return 2;
        }
        else if(cards_hand[i].name == "Dynamit")
        {
            cards_hand[i].dyn_active = true;
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }

        //Modre
        else if(cards_hand[i].range > Ai::has_gun(cards_desk) && Ai::best_gun(cards_hand) == static_cast<int>(i))
        {
            //vyhodit starou zbran
            for(size_t j = 0; j < cards_desk.size(); j++)
            {
                if(cards_desk[j].range != 0)
                {
                    Ai::discard_card(g, cards_hand, cards_desk,j + cards_hand.size());
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
                Ai::index(cards_desk, cards_hand[i].name) == -1 && cards_hand[i].name != "Vezeni")
        {
            cards_desk.push_back(cards_hand[i]);
            cards_hand.erase(cards_hand.begin() + i);
            return 2;
        }

        //neutralni karty: hokynarstvi, salon, kulomet, indiani
        else if(cards_hand[i].card_type == "neu" && Ai::play_neu(g, cards_hand[i].name))
        {
           g->deck.push_back(cards_hand[i]);
           cards_hand.erase(cards_hand.begin() + i);
           if(g->deck.back().name == "Salon")
           {
               GameTools::saloon(g);
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
                    if(Ai::panika_balou_play(g, p))
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
                        played_bang = (Ai::index(cards_desk, "Volcanic") != -1 ||
                                name == "willy") ? false : true;
                        return 1;
                    }
                }
            }
            if(name == "Panika")
            {
                for(size_t j = 0; j < g->game_order.size(); j++)
                {
                    //muzeme pouzit jenom na vzdalenost mensi rovno 1
                    if(enemies_id.find(g->game_order[j]->id) != enemies_id.end() &&
                        Ai::can_play_panika(g, id, g->game_order[j]->id) && Ai::panika_balou_play(g, g->game_order[j]->id))
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
            bool result = (Ai::discard_card(g, cards_hand, "neu") ? true : false);
            result = (result ? true : Ai::discard_card(g, cards_hand, "def"));
            result = (result ? true : Ai::discard_blue(g, cards_hand));
            result = (result ? true : Ai::discard_card(g, cards_hand, "agr"));
        }
        else
        {
            bool result = (Ai::discard_card(g, cards_hand, "neu") ? true : false);
            result = (result ? true : Ai::discard_blue(g, cards_hand));
            result = (result ? true : Ai::discard_card(g, cards_hand, "agr"));
            result = (result ? true : Ai::discard_card(g, cards_hand, "def"));
        }
    }
}
bool Player::resolve_jail()
{
    if(Ai::index(cards_desk, "Vezeni") == -1)
    {
        return true;
    }

    Card c = g->draw_from_deck();
    bool result = (c.suit == "Srdce" ? true : false);
    g->deck.push_back(c);
    g->deck.push_back(cards_desk[Ai::index(cards_desk, "Vezeni")]);
    cards_desk.erase(cards_desk.begin() + Ai::index(cards_desk, "Vezeni"));
    return result;
}
bool Player::resolve_dyn()
{
    //pokud tuto kartu nemame vubec, tak muzeme se muzeme chovat jakoby dynamit nebouchl
    if(Ai::index(cards_desk, "Dynamit") == -1)
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

        //dynamit je aktivni a pripraven bouchnout
        cards_desk[Ai::index(cards_desk, "Dynamit")].dyn_active = true;
        g->game_order[next]->cards_desk.push_back(cards_desk[Ai::index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + Ai::index(cards_desk, "Dynamit"));
    }
    else
    {
        //dynamit bude doutnat po dalsim nasazeni
        cards_desk[Ai::index(cards_desk, "Dynamit")].dyn_active = false;
        g->deck.push_back(cards_desk[Ai::index(cards_desk, "Dynamit")]);
        cards_desk.erase(cards_desk.begin() + Ai::index(cards_desk, "Dynamit"));
    }
    return result;
}
bool Player::resolve_barrel()
{
    if(Ai::index(cards_desk, "Barel") == -1)
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
    //vynuceni zahrani karty bang nebo ekvivalentu
    bool res = false;
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].name == "Bang")
        {
            Ai::discard_card(g, cards_hand, cards_desk, Ai::index(cards_hand, "Bang"));
            res = true;
            break;
        }
    }

    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(g->mode == "Duel" && id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
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
                Ai::discard_card(g, cards_hand, cards_desk, Ai::index(cards_hand, "Vedle"));
                barel = true;
                break;
            }
        }
    }

    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(g->mode == "Bang" || g->mode == "Vedle" || g->mode == "Slab")
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
        }
    }

    return barel;
}
bool Player::resolve_slab_bang()
{
    //zde musime zahrat 2x vedle nebo ekvivalent, napr 2x barel pokud to vyjde

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

    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    enemies_id.insert(g->game_order[g->active_player]->id);
    if(role == 'S')
    {
        Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
    }

    if(vedle + hand_vedle >= 2)
    {
        while(vedle < 2)
        {
            Ai::discard_card(g, cards_hand, cards_desk, Ai::index(cards_hand, "Vedle"));
            vedle++;
        }
        return true;
    }
    return false;
}
void Player::dostavnik_wells(int count)
{
    Card c;
    for (int i = 0; i < count; i++)
    {
        c = g->draw_from_deck();
        cards_hand.push_back(c);
    }
}
bool Player::has_notai_ability()
{
    //jenom tito muzou pouzit schopnost jako notAI
    return name == "jourd" || name == "ketchum" || name == "pedro" || name == "jesse" || name == "carlson";
}
bool Player::has_dyn()
{
    return Ai::index(cards_desk, "Dynamit") != -1 && cards_desk[Ai::index(cards_desk, "Dynamit")].dyn_active;
}
bool Player::has_jail()
{
    return Ai::index(cards_desk, "Vezeni") != -1;
}
bool Player::dec_hp(int lifes)
{
    health -= lifes;
    int beers = 0;

    //pokud mam v ruce piva, tak se jeste muzu zachranit
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
                Ai::discard_card(g, cards_hand, cards_desk, Ai::index(cards_hand, "Pivo"));
                health++;
            }
        }
    }



    return health > 0;
}
string Player::file_loc()
{
    return ":/char_img/char_img/" + name + ".png";
}
string Player::role_loc()
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
    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
        }
    }

    size_t max = cards_hand.size() + cards_desk.size() - 1;

    //nadhodny generator pro vsechny karty, na stole i v ruce
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,max);

    size_t i = dist(rng);
    if(i < cards_hand.size())
    {
        Card c;
        c = cards_hand[i];
        cards_hand.erase(cards_hand.begin() + i);
        if(c.name == "Dynamit")
        {
            c.dyn_active = false;
        }
        return c;
    }
    else
    {
        Card c;
        c = cards_desk[i - cards_hand.size()];
        cards_desk.erase(cards_desk.begin() + i - cards_hand.size());
        if(c.name == "Dynamit")
        {
            c.dyn_active = false;
        }
        return c;
    }
}
Card Player::give_random_card_hand()
{
    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
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
int Player::choose(const std::vector<Card>& cards)
{
    //pouziva se pri vyberu karet, kdyz je Hokynarstvi nebo v schopnosti Kit Carlson
    if(Ai::index(cards, "WellsFargo") != -1)
    {
        return Ai::index(cards, "WellsFargo");
    }
    else if(Ai::index(cards, "Dostavnik") != -1)
    {
        return Ai::index(cards, "Dostavnik");
    }
    else if(health > max_health / 2)
    {
        if(Ai::index(cards, "agr") != -1)
        {
            return Ai::index(cards, "agr");
        }
        else if(Ai::index(cards, "def") != -1)
        {
            return Ai::index(cards, "def");
        }
        else
        {
            return Ai::index(cards, "neu");
        }
    }
    else
    {
        if(Ai::index(cards, "def") != -1)
        {
            return Ai::index(cards, "def");
        }
        else if(Ai::index(cards, "agr") != -1)
        {
            return Ai::index(cards, "agr");
        }
        else
        {
            return Ai::index(cards, "neu");
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
int Player::exist_enemy_jail()
{
    //nalezeni nepritele, ktereho bychom mohli posadit do vezenis
    for(auto p : enemies_id)
    {
        if(p != g->game_order[0]->id)
        {
            int pos = g->id_to_pos(p);
            if(Ai::index(g->game_order[pos]->cards_desk, "Vezeni") == -1)
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
std::vector<Card> Player::give_all_cards()
{
    //odevzdani vsech karet ze stolu a z ruky, napr kdyz umrel nebo serif zabil Vice
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
    //defaultni reset vsech hodnot aktivniho hrace
    played_bang = false;
    played_vedle = 0;
    discarded = 0;
    drawed = false;
    target_id = -1;
    ability_used = false;
    barel = 0;
    if(Ai::index(cards_desk, "Dynamit") != -1)
    {
        cards_desk[Ai::index(cards_desk, "Dynamit")].dyn_active = true;
    }
}
