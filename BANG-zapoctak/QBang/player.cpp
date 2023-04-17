#include "game.h"
#include "ai.h"
#include "gametools.h"

using namespace std;

void Player::draw_phase()
{
    pd.discarded = 0;
    Ai::enemy_check(pd);

	Card c;
    //ze zacatku se lize 2 karty
	for (size_t i = 0; i < 2; i++)
	{
        c = pd.g->draw_from_deck();
        pd.cards_hand.push_back(c);
	}
    pd.drawed = true;
}
bool Player::game_phase()
{
    for(size_t i = 0; i < pd.cards_hand.size(); i++)
    {
        if(pd.cards_hand[i].play(pd.g->active_player, pd))
        {
            pd.g->deck.push_back(pd.cards_hand[i]);
            pd.cards_hand.erase(pd.cards_hand.begin() + i);
            return true;
        }

        else if(pd.cards_hand[i].mode == VEZENI && exist_enemy_jail() != -1)
        {
            //musi existovat nekdo komu muzeme predat vezeni a je nas nepritel, nesmime to dat serifovi
            pass_jail(i, exist_enemy_jail());
            return true;
        }
        else if(pd.cards_hand[i].mode == DYNAMIT)
        {
            pd.cards_hand[i].dyn_active = true;
            cards_desk.push_back(pd.cards_hand[i]);
            pd.cards_hand.erase(pd.cards_hand.begin() + i);
            return true;
        }
        //Modre
        else if(pd.cards_hand[i].range > Ai::has_gun(cards_desk) && Ai::best_gun(pd.cards_hand) == static_cast<int>(i))
        {
            //vyhodit starou zbran
            for(size_t j = 0; j < cards_desk.size(); j++)
            {
                if(cards_desk[j].range != 0)
                {
                    Ai::discard_card(pd.g, pd.cards_hand, cards_desk, j + pd.cards_hand.size());
                }
            }

            //nasadit novou
            cards_desk.push_back(pd.cards_hand[i]);
            pd.cards_hand.erase(pd.cards_hand.begin() + i);
            if(cards_desk.back().mode == VOLCANIC)
            {
                pd.played_bang = false;
            }
            return true;
        }
        else if(pd.cards_hand[i].edge == 'M' && pd.cards_hand[i].range == 0 &&
                Ai::index_name(cards_desk, pd.cards_hand[i].mode) == -1 && pd.cards_hand[i].mode != VEZENI)
        {
            cards_desk.push_back(pd.cards_hand[i]);
            pd.cards_hand.erase(pd.cards_hand.begin() + i);
            return true;
        }
    }  
    return false;
}
void Player::discard_phase()
{
    /*
    * > max_health/2 => odhazuju nejdriv 'neu' karty pak 'def' pak 'M' pak 'agr'
    * <= max_health/2 => odhazuju nejriv 'neu' pak 'M' pak 'agr' pak 'def'
    */
    if (pd.cards_hand.size() <= (size_t)health)
    {
        return;
    }

    while (pd.cards_hand.size() > (size_t)health)
    {
        //jsme agresivni
        if (health > max_health / 2)
        {
            bool result = (Ai::discard_card(pd.g, pd.cards_hand, NEU) ? true : false);
            result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, DEF));
            result = (result ? true : Ai::discard_blue(pd.g, pd.cards_hand));
            result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, AGR));
        }
        //jsme defensivni
        else
        {
            bool result = (Ai::discard_card(pd.g, pd.cards_hand, NEU) ? true : false);
            result = (result ? true : Ai::discard_blue(pd.g, pd.cards_hand));
            result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, AGR));
            result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, DEF));
        }
    }
}
bool Player::resolve_jail()
{
    if(Ai::index_name(cards_desk, VEZENI) == -1)
    {
        return true;
    }

    Card c = pd.g->draw_from_deck();
    bool result = c.suit == SRDCE;
    pd.g->deck.push_back(cards_desk[Ai::index_name(cards_desk, VEZENI)]);
    cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, VEZENI));
    pd.g->deck.push_back(c);
    return result;
}
bool Player::resolve_dyn()
{
    //pokud tuto kartu nemame vubec, tak muzeme se muzeme chovat jakoby dynamit nebouchl
    if(Ai::index_name(cards_desk, DYNAMIT) == -1)
    {
        return false;
    }

    Card c = pd.g->draw_from_deck();
    bool result = c.rank >= 2 && c.rank <= 9 && c.suit == PIKY;
    pd.g->deck.push_back(c);

    size_t next = 0;
    if(!result)//prehodit dalsimu hraci, kdyz nebouchl
    {
        next = (pd.g->active_player + 1) % pd.g->player_alive;
        //dynamit je aktivni a pripraven bouchnout
        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = true;
        pd.g->game_order[next]->cards_desk.push_back(cards_desk[Ai::index_name(cards_desk, DYNAMIT)]);
    }
    else
    {
        //dynamit bude doutnat po dalsim nasazeni
        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = false;
        pd.g->deck.push_back(cards_desk[Ai::index_name(cards_desk, DYNAMIT)]);        
    }  
    cards_desk.erase(cards_desk.begin() + Ai::index_name(cards_desk, DYNAMIT));

    return result;
}
bool Player::resolve_barrel()
{
    if(Ai::index_name(cards_desk, BAREL) == -1)
    {
        return false;
    }
    Card c = pd.g->draw_from_deck();
    bool result = (c.suit == SRDCE ? true : false);
    pd.g->deck.push_back(c);
    return result;
}
bool Player::play_bang()
{
    //vynuceni zahrani karty bang nebo ekvivalentu
    bool res = false;
    for(size_t i = 0; i < pd.cards_hand.size(); i++)
    {
        if(pd.cards_hand[i].mode == BANG)
        {
            Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, BANG));
            res = true;
            break;
        }
    }

    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(pd.g->mode == DUEL && id != pd.g->game_order[pd.g->active_player]->id)
    {
        add_enemy();
    }
    return res;
}
bool Player::play_vedle()
{
    bool barel = resolve_barrel();

    if(!barel)
    {
        for(size_t i = 0; i < pd.cards_hand.size(); i++)
        {
            if(pd.cards_hand[i].mode == VEDLE)
            {
                Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, VEDLE));
                barel = true;
                break;
            }
        }
    }
    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    if(pd.g->mode != KULOMET)
    {
        add_enemy();
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
    for(size_t i = 0; i < pd.cards_hand.size(); i++)
    {
        if(pd.cards_hand[i].mode == VEDLE)
        {
            hand_vedle++;
        }
    }

    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    add_enemy();

    //musime odhodit 2 karty vedle nebo jejich ekvivalent, jinak ztracime zivot
    if(vedle + hand_vedle >= 2)
    {
        while(vedle < 2)
        {
            Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, VEDLE));
            vedle++;
        }
        return true;
    }
    return false;
}
bool Player::has_notai_ability()
{
    //jenom tito muzou pouzit schopnost jako notAI
    return (Chars)pd.ranking == JOURD || (Chars)pd.ranking == KETCHUM ||
           (Chars)pd.ranking == PEDRO || (Chars)pd.ranking == JESSE || (Chars)pd.ranking == CARLSON;
}
bool Player::has_dyn()
{
    return Ai::index_name(cards_desk, DYNAMIT) != -1 && cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active;
}
bool Player::has_jail()
{
    return Ai::index_name(cards_desk, VEZENI) != -1;
}
size_t Player::hand_size()
{
    return pd.cards_hand.size();
}
PlayerData& Player::data()
{
    return pd;
}
bool Player::dec_hp(int lifes)
{
    health -= lifes;
    int beers = 0;

    //pokud mam v ruce piva, tak se jeste muzu zachranit
    if(health <= 0)
    {
        for(size_t i = 0; i < pd.cards_hand.size(); i++)
        {
            if(pd.cards_hand[i].mode == PIVO)
            {
                beers++;
            }
        }
        if(health + beers > 0)
        {
            while(health <= 0)
            {
                Ai::discard_card(pd.g, pd.cards_hand, cards_desk, Ai::index_name(pd.cards_hand, PIVO));
                health++;
            }
        }
    }
    return health > 0;
}
string Player::file_loc()
{
    string name = Names[pd.ranking];
    return ":/char_img/char_img/" + name + ".png";
}
string Player::role_loc()
{
    bool end = pd.g->finished();
    switch(pd.role)
    {
        case 'S':
            return ":/cards/cards/sheriff.png";
        case 'V':
            return (health > 0  && pd.isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/deputy.png");
        case 'O':
            return (health > 0  && pd.isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/renegade.png");
        case 'B':
            return (health > 0  && pd.isai && !end ? ":/cards/cards/back-role.png" : ":/cards/cards/outlaw.png");
        default:
            break;
    }
    return "";
}
void Player::set_enemy(int sheriff, const vector<int>& ids)
{
    switch (pd.role)
	{
	case 'O':
		for (auto&& pl : ids)
		{
			if (pl != sheriff && pl != id)
			{
                pd.enemies_id.insert(pl);
			}
		}
		break;
	case 'B':
        pd.enemies_id.insert(sheriff);
		break;
	case 'V':
        //pokud je pocet hracu roven 5 nebo 6, tak vime jiste kdo jsou nasi nepratele
        if (pd.g->player_count == 5 || pd.g->player_count == 6)
		{
			for (auto&& pl : ids)
			{
				if (pl != sheriff && pl != id)
				{
                    pd.enemies_id.insert(pl);
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
    add_enemy();

    size_t max = pd.cards_hand.size() + cards_desk.size() - 1;
    //nadhodny generator pro vsechny karty, na stole i v ruce
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,max);

    size_t i = dist(rng);
    Card c;
    if(i < pd.cards_hand.size())
    {     
        c = pd.cards_hand[i];
        pd.cards_hand.erase(pd.cards_hand.begin() + i);     
    }
    else
    {
        c = cards_desk[i - pd.cards_hand.size()];
        cards_desk.erase(cards_desk.begin() + i - pd.cards_hand.size());
    }

    c.dyn_active = false;
    return c;
}
Card Player::give_random_card_hand()
{
    //zapsani na seznam nepratel + pokud se strili na serifa tak i jeho pomocnici si zaznamenaji
    add_enemy();

    size_t max = pd.cards_hand.size() - 1;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,max);

    size_t i = dist(rng);
    Card c;
    c = pd.cards_hand[i];
    pd.cards_hand.erase(pd.cards_hand.begin() + i);
    return c;
}
int Player::choose(const std::vector<Card>& cards)
{
    //pouziva se pri vyberu karet, kdyz je Hokynarstvi nebo v schopnosti Kit Carlson
    if(Ai::index_name(cards, WELLSFARGO) != -1)
    {
        return Ai::index_name(cards, WELLSFARGO);
    }
    else if(Ai::index_name(cards, DOSTAVNIK) != -1)
    {
        return Ai::index_name(cards, DOSTAVNIK);
    }
    else if(health > max_health / 2)
    {
        if(Ai::index_type(cards, AGR) != -1)
        {
            return Ai::index_type(cards, AGR);
        }
        else if(Ai::index_type(cards, DEF) != -1)
        {
            return Ai::index_type(cards, DEF);
        }
        else
        {
            return Ai::index_type(cards, NEU);
        }
    }
    else
    {
        if(Ai::index_type(cards, DEF) != -1)
        {
            return Ai::index_type(cards, DEF);
        }
        else if(Ai::index_type(cards, AGR) != -1)
        {
            Ai::index_type(cards, AGR);
        }
        else
        {
            return Ai::index_type(cards, NEU);
        }
    }
    return 0;
}
void Player::set_target_id(const std::string& name)
{
    for(size_t i = 0; i < pd.g->game_order.size(); i++)
    {
        if(Names[pd.g->game_order[i]->pd.ranking] == name)
        {
            target_id = pd.g->game_order[i]->id;
            return;
        }
    }
    target_id = -1;
}
void Player::take_card(Card &c)
{
    pd.cards_hand.push_back(c);
}
int Player::exist_enemy_jail()
{
    //nalezeni nepritele, ktereho bychom mohli posadit do vezenis
    for(auto p : pd.enemies_id)
    {
        if(p != pd.g->game_order[0]->id)
        {
            int pos = GameTools::id_to_pos(pd.g, p);
            if(Ai::index_name(pd.g->game_order[pos]->cards_desk, VEZENI) == -1)
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
    c = pd.cards_hand[c_index];
    pd.cards_hand.erase(pd.cards_hand.begin() + c_index);

    for(size_t i = 0; i < pd.g->game_order.size(); i++)
    {
        if(pd.g->game_order[i]->id == enemy_id)
        {
            pd.g->game_order[i]->cards_desk.push_back(c);
            pd.g->game_order[i]->pd.enemies_id.insert(id);
        }
    }
}
std::vector<Card> Player::give_all_cards()
{
    //odevzdani vsech karet ze stolu a z ruky, napr kdyz umrel nebo serif zabil Vice
    vector<Card> v;
    while(pd.cards_hand.size() > 0)
    {
        v.push_back(pd.cards_hand[0]);
        pd.cards_hand.erase(pd.cards_hand.begin());
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
    pd.played_bang = false;
    pd.played_vedle = 0;
    pd.discarded = 0;
    pd.drawed = false;
    target_id = -1;
    pd.ability_used = false;
    pd.barel = 0;
    if(Ai::index_name(cards_desk, DYNAMIT) != -1)
    {
        cards_desk[Ai::index_name(cards_desk, DYNAMIT)].dyn_active = true;
    }
}
void Player::add_enemy()
{
    pd.enemies_id.insert(pd.g->game_order[pd.g->active_player]->id);
    if(pd.role == 'S')
    {
        Ai::vice_add_enemy(pd.g, pd.g->game_order[pd.g->active_player]->id);
    }
}
