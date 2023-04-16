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

bool Ai::discard_card(Game *g, std::vector<Card> &cards, int type)
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

int Ai::index_type(const std::vector<Card> &cards, int type)
{
    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].card_type == type)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int Ai::index_name(const std::vector<Card> &cards, int name)
{
    //vrati index teto karty v cards nebo -1, jestli karta neexistuje
    for(size_t i = 0; i < cards.size(); i++)
    {
        if(cards[i].mode == name)
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

bool Ai::play_neu(Game *g, int name)
{
    //ma-li AI zahrat neutralni kartu?
    int s = 0;
    int o = 0;
    int b = 0;
    int v = 0;

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        switch(g->game_order[i]->say_role())
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

    if(name == HOKYNARSTVI || name == SALON)
    {
        switch(g->game_order[g->active_player]->say_role())
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

        switch(g->game_order[g->active_player]->say_role())
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

void Ai::vice_add_enemy(Game* g, int enemy_id)
{
    //pokud nekdo utoci na serifa, tak i jeho pomocnici si ulozi tohoto hrace do seznamu nepratel
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->say_role() == 'V')
        {
            g->game_order[i]->pd.enemies_id.insert(enemy_id);
        }
    }
}

bool Ai::panika_balou_play(Game *g, int enemy_id)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == enemy_id)
        {
            //musi mit nejake karty v ruce nebo na stole, jinak pouziti Paniky nebo Cat Balou nema smysl
            return g->game_order[i]->pd.cards_hand.size() + g->game_order[i]->cards_desk.size() > 0;
        }
    }
    return false;
}

void Ai::cringo_abil(Game *g)
{
    int cringo_pos = -1;
    int cringo_ranking = 15;

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->pd.ranking == cringo_ranking)
        {
            cringo_pos = i;
            break;
        }
    }

    if(g->game_order[g->active_player]->pd.cards_hand.size() > 0)
    {
        g->game_order[cringo_pos]->pd.cards_hand.push_back(g->game_order[g->active_player]->give_random_card_hand());
    }
}

bool Ai::jesse_abil(Game *g)
{
    int index = -1;
    int cards = -1;
    //najde mezi neprateli nejvetsi pocet karet v ruce a tam odebere kartu
    if(g->game_order[g->active_player]->pd.enemies_id.size() != 0)
    {
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(g->game_order[g->active_player]->pd.enemies_id.find(g->game_order[i]->id) != g->game_order[g->active_player]->pd.enemies_id.end() &&
                g->game_order[i]->pd.cards_hand.size() > (size_t)cards)
            {
                cards = g->game_order[i]->pd.cards_hand.size();
                index = i;
            }
        }
    }

    if(cards == -1 || cards == 0)
    {
        return false;
    }
    else
    {
        g->game_order[g->active_player]->pd.ability_used = true;
        g->game_order[g->active_player]->pd.cards_hand.push_back(g->game_order[index]->give_random_card_hand());
        g->game_order[g->active_player]->pd.cards_hand.push_back(g->draw_from_deck());

        return true;
    }
}

bool Ai::no_pedro_abil(Game *g, int id)
{
    size_t pedro_pos = 0;
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            pedro_pos = i;
            break;
        }
    }

    return g->game_order[g->active_player]->pd.isai || g->game_order[pedro_pos]->pd.drawed || g->mode != NONE;
}

bool Ai::no_jourd_abil(Game *g, int barel)
{
    return ((Chars)g->game_order[g->active_player]->pd.ranking == SLAB && barel == 2) ||
           ((Chars)g->game_order[g->active_player]->pd.ranking != SLAB && barel == 1);
}

bool Ai::bang(int position, PlayerData& pd)
{
    Game* g = pd.g;

    for(size_t j = 0; j < g->game_order.size(); j++)
    {
        if(pd.enemies_id.find(g->game_order[j]->id) != pd.enemies_id.end() &&
            g->distances.find(g->game_order[position]->id)->second[g->game_order[j]->id] <= 1)
        {
            g->game_order[position]->target_id = g->game_order[j]->id;
            pd.played_bang = (Ai::index_name(g->game_order[position]->cards_desk, VOLCANIC) != -1 ||
                              pd.ranking == WILLY) ? false : true;

            pd.g->mode = (pd.ranking == SLAB ? SLAB_BANG : BANG);
            return true;
        }
    }
    return false;
}

bool Ai::beer(int position, Game *g)
{
    if(g->game_order[position]->health < g->game_order[position]->max_health)
    {
        g->game_order[position]->health++;
        return true;
    }
    return false;
}

bool Ai::neu(Game* g, int mode)
{
    switch(mode)
    {
    case HOKYNARSTVI:
        if(play_neu(g, HOKYNARSTVI))
        {
            GameTools::load_emporio(g);
            g->mode = HOKYNARSTVI;
            return true;
        }
        break;
    case INDIANI:
        if(play_neu(g, INDIANI))
        {
            g->mode = INDIANI;
            return true;
        }
        break;
    case KULOMET:
        if(play_neu(g, KULOMET))
        {
            g->mode = KULOMET;
            return true;
        }
        break;
    case SALON:
        if(play_neu(g, SALON))
        {
            GameTools::saloon(g);
            return true;
        }
    }

    return false;
}

bool Ai::duel(PlayerData &pd, int& target_id)
{
    if(pd.enemies_id.size() != 0)
    {
        target_id = *pd.enemies_id.begin();
        pd.g->duel_active_turn = false;
        pd.g->mode = DUEL;
        return true;
    }

    return false;
}

bool Ai::balou(PlayerData &pd, int& target_id)
{
    for(auto p : pd.enemies_id)
    {
        if(Ai::panika_balou_play(pd.g, p))
        {
            target_id = p;
            pd.g->mode = BALOU;
            return true;
        }
    }

    return false;
}

bool Ai::panika(PlayerData &pd, int &target_id, int id)
{
    for(size_t j = 0; j < pd.g->game_order.size(); j++)
    {
        //muzeme pouzit jenom na vzdalenost mensi rovno 1
        if(pd.enemies_id.find(pd.g->game_order[j]->id) != pd.enemies_id.end() &&
            Ai::can_play_panika(pd.g, id, pd.g->game_order[j]->id) && Ai::panika_balou_play(pd.g, pd.g->game_order[j]->id))
        {
            target_id = pd.g->game_order[j]->id;
            pd.g->mode = PANIKA;
            return true;
        }
    }
    return false;
}
