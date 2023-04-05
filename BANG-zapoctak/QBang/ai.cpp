#include "game.h"
#include "ai.h"

bool Ai::discard_blue(Game *g, std::vector<Card> &cards)
{
    //vyhozeni jakekoli modre karty z ruky
    for (size_t i = 0; i < cards.size(); i++)
    {
        if (cards[i].edge == 'M')
        {
            g->deck.push_back(cards[i]);
            cards.erase(cards.begin() + i);
            return true;
        }
    }
    return false;
}

bool Ai::discard_card(Game *g, std::vector<Card> &cards, const std::string &type)
{
    //odhozeni karty s typem type
    for (size_t i = 0; i < cards.size(); i++)
    {
        if (cards[i].card_type == type)
        {
            g->deck.push_back(cards[i]);
            cards.erase(cards.begin() + i);
            return true;
        }
    }
    return false;
}

void Ai::discard_card(Game* g, std::vector<Card>& c_hand, std::vector<Card>& c_desk, size_t index)
{
    if(index >= c_hand.size())
    {
        size_t i = index - c_hand.size();
        g->deck.push_back(c_desk[i]);
        c_desk.erase(c_desk.begin() + i);
    }
    else
    {
        g->deck.push_back(c_hand[index]);
        c_hand.erase(c_hand.begin() + index);
    }
}

int Ai::index(const std::vector<Card> &cards, const std::string &name_type)
{
    //vrati index teto karty v cards nebo -1, jestli karta neexistuje
    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].name == name_type || cards[i].card_type == name_type)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool Ai::can_play_panika(Game* g, int id, int enemy_id)
{
    //vzdalenost v multigrafu musi byt mensi rovno 1
    int gun = -1;
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            gun = has_gun(g->game_order[i]->cards_desk);
            break;
        }
    }

    gun = (gun == -1 ? 0 : gun - 1);
    return g->distances.find(id)->second[enemy_id] + gun <= 1;
}

bool Ai::play_neu(Game *g, const std::string &name)
{
    //ma-li AI zahrat neutralni kartu?
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
        switch(g->game_order[g->active_player]->role)
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
        //utocne karty vzdy zahrajeme
        if(g->player_alive == 2)
        {
            return true;
        }

        switch(g->game_order[g->active_player]->role)
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

int Ai::best_gun(std::vector<Card> &cards)
{
    //nalezeni nejlepsi zbrane v ruce, napr kdyz chceme si vylepsit docasnou zbran, kterou mame
    int best = 0;
    int index = -1;
    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].range > best)
        {
            best = cards[i].range;
            index = i;
        }
    }
    return index;
}

int Ai::has_gun(std::vector<Card> &cards)
{
    //vrati -1, pokud hrac nema zbran, anebo range teto zbrane

    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].range != 0)
        {
            return cards[i].range;
        }
    }
    return -1;
}
