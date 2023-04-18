#include "game.h"
#include "ai.h"

#include <algorithm>

using namespace std;

void GameTools::load_chars(Game* g, std::vector<Hrac>& characters)
{
    //nacteni vsech moznych hracu
    Hrac pl = make_unique<Bart>(g);
    characters.push_back(move(pl));
    pl = make_unique<Blackj>(g);
    characters.push_back(move(pl));
    pl = make_unique<Calamity>(g);
    characters.push_back(move(pl));
    pl = make_unique<Carlson>(g);
    characters.push_back(move(pl));
    pl = make_unique<Cringo>(g);
    characters.push_back(move(pl));
    pl = make_unique<Jesse>(g);
    characters.push_back(move(pl));
    pl = make_unique<Jourd>(g);
    characters.push_back(move(pl));
    pl = make_unique<Ketchum>(g);
    characters.push_back(move(pl));
    pl = make_unique<Lucky>(g);
    characters.push_back(move(pl));
    pl = make_unique<Paul>(g);
    characters.push_back(move(pl));
    pl = make_unique<Pedro>(g);
    characters.push_back(move(pl));
    pl = make_unique<Rose>(g);
    characters.push_back(move(pl));
    pl = make_unique<Slab>(g);
    characters.push_back(move(pl));
    pl = make_unique<Suzy>(g);
    characters.push_back(move(pl));
    pl = make_unique<Vulture>(g);
    characters.push_back(move(pl));
    pl = make_unique<Willy>(g);
    characters.push_back(move(pl));

    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };

    size_t shuffle_count = 20;
    for (size_t i = 0; i < shuffle_count; i++)
    {
        shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav
    }
}

void GameTools::load_card(Game* g, std::vector<std::string> &v)
{
    string name = v[0];
    int suit = stoi(v[1]);
    int rank = stoi(v[2]);
    char edge = v[3][0];
    int id = stoi(v[4]);
    int card_type = stoi(v[5]);
    int range = stoi(v[6]);
    int mode = stoi(v[7]);

    Card card(id, name, edge, suit, rank, card_type, range, mode);

    g->deck.push_back(card);

    //po kazde nove karte premichame balicek
    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };
    shuffle(begin(g->deck), end(g->deck), rng);//pro nahodny vyber karet
}

void GameTools::rm_enemy(Game* g, int id)
{
    //kdykoli nekdo umre, tak nema smysl uchovavat tohoto hrace v seznamu nepratel

    for(size_t i = 0; i <g->game_order.size(); i++)
    {
        g->game_order[i]->data().enemies_id.erase(id);
    }
}

void GameTools::rotate_serif(Game* g)
{
    while (g->game_order[0]->data().role != 'S')
    {
        rotate(g->game_order.begin(), g->game_order.begin() + 1, g->game_order.end());
    }

    g->active_player = 0;
}

void GameTools::saloon(Game* g)
{
    //vsem +1 zivot, pokud jiz nemaji max. pocet
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->max_health > g->game_order[i]->health)
        {
            g->game_order[i]->health++;
        }
    }
}

void GameTools::set_notai(Game* g)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(!g->game_order[i]->data().isai)
        {
            g->notai = i;
            return;
        }
    }
    g->notai = 0;
}

void GameTools::load_emporio(Game* g)
{
    //byla zahrana karta Hokynarstvi
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        g->emporio.push_back(g->draw_from_deck());
    }
}

int GameTools::id_to_pos(Game* g, int id)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void GameTools::suzy_abil(Game* g)
{
    int suzy_ranking = 14;

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->data().ranking == suzy_ranking)
        {
            g->game_order[i]->ability();
            return;
        }
    }
}

void GameTools::jourd_abil(Game *g)
{
    int jourd_ranking = JOURD;

    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->data().ranking == jourd_ranking)
        {
            g->game_order[i]->data().barel = 0;
            return;
        }
    }
}

size_t GameTools::hand_size(Game* g, int id)
{
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            return g->game_order[i]->data().cards_hand.size();
        }
    }
    return 0;
}

string GameTools::id_to_name(Game* g, int id)
{
    if(id == -1)
    {
        return "";
    }
    for(size_t i = 0; i < g->game_order.size(); i++)
    {
        if(g->game_order[i]->id == id)
        {
            return Names[g->game_order[i]->data().ranking];
        }
    }
    return "";
}

void GameTools::dostavnik_wells(Game *g, int position, int count)
{
    Card c;
    for(int i = 0; i < count; i++)
    {
        c = g->draw_from_deck();
        g->game_order[position]->take_card(c);
    }
}

void GameTools::discard_killed(Game* g, char role, int pos)
{
    //nekdo zabil banditu
    if(role == 'B' && g->deck.back().mode != DYNAMIT && g->mode != DUEL)
    {
        int bounty = 3;
        for(int i = 0; i < bounty; i++)
        {
            g->game_order[g->active_player]->data().cards_hand.push_back(g->draw_from_deck());
        }
    }
    //zabit pri duelu
    else if(role == 'B' && g->mode == DUEL)
    {
        int bounty = 3;
        if(g->duel_active_turn)
        {
            for(int i = 0; i < bounty; i++)
            {
                int killed_by = g->game_order[pos]->target_id;
                int duel_killer = GameTools::id_to_pos(g, killed_by);
                g->game_order[duel_killer]->data().cards_hand.push_back(g->draw_from_deck());
            }
        }
        else
        {
            for(int i = 0; i < bounty; i++)
            {
                g->game_order[g->active_player]->data().cards_hand.push_back(g->draw_from_deck());
            }
        }
    }
    //Serif zabil sveho pomocnika
    else if(role == 'V' && g->game_order[g->active_player]->data().role == 'S' && g->deck.back().mode != DYNAMIT)
    {
        vector<Card> shame = g->game_order[g->active_player]->give_all_cards();
        for(size_t i = 0; i < shame.size(); i++)
        {
            g->deck.push_back(shame[i]);
        }
    }
}

void GameTools::resolve_played_card(Game *g)
{
    switch(g->mode)
    {
    case PANIKA:
        panika(g);
        break;
    case BALOU:
        balou(g);
        break;
    case INDIANI:
        indiani_kulomet(g);
        break;
    case KULOMET:
        indiani_kulomet(g);
        break;
    case HOKYNARSTVI:
        hokynarstvi(g);
        break;
    case BANG:
        bang(g);
        break;
    case VEDLE:
        bang(g);
        break;
    case SLAB_BANG:
        bang(g);
        break;
    case DUEL:
        duel(g);
        break;
    default:
        break;
    }
}

void GameTools::panika(Game *g)
{
    int enemy_id = g->game_order[g->active_player]->target_id;

    if(Ai::can_play_panika(g, g->game_order[g->active_player]->id, enemy_id))
    {
        int pos = id_to_pos(g, enemy_id);
        Card c = g->game_order[pos]->give_random_card();
        g->game_order[g->active_player]->data().cards_hand.push_back(c);
    }
    g->mode = NONE;
}

void GameTools::balou(Game *g)
{
    int pos = id_to_pos(g, g->game_order[g->active_player]->target_id);
    Card c = g->game_order[pos]->give_random_card();
    g->deck.push_back(c);
    g->mode = NONE;
}

void GameTools::indiani_kulomet(Game *g)
{
    if(g->neu_turn == -1)
    {
        g->neu_turn = (g->active_player + 1) % g->player_alive;
        return;
    }

    bool result = false;
    if(!g->game_order[g->neu_turn]->data().isai)
    {
        result = false;
    }
    else if(g->mode == INDIANI)
    {
        result = g->game_order[g->neu_turn]->play_bang();
    }
    else if(g->mode == KULOMET)
    {
        result = g->game_order[g->neu_turn]->play_vedle();
    }
    int react = g->neu_turn;
    g->next_neu();

    if(!result)
    {
        bool hp = g->game_order[react]->dec_hp(1);
        if(!hp)
        {
            g->killed(g->game_order[react]->id);
        }
    }
}

void GameTools::hokynarstvi(Game *g)
{
    if(g->neu_turn == -1)
    {
        g->neu_turn = g->active_player;
        return;
    }
    if(g->neu_turn != -1 && !g->game_order[g->neu_turn]->data().isai)
    {
        return;
    }
    int choice = g->game_order[g->neu_turn]->choose(g->emporio);
    g->game_order[g->neu_turn]->data().cards_hand.push_back(g->emporio[choice]);
    g->emporio.erase(g->emporio.begin() + choice);

    g->next_neu();
}

void GameTools::bang(Game *g)
{
    int enemy_id = g->game_order[g->active_player]->target_id;
    int pos = GameTools::id_to_pos(g, enemy_id);

    bool vedle;
    if(!g->game_order[pos]->data().isai)
    {
        vedle = false;
        g->game_order[pos]->data().barel = 0;
        g->game_order[pos]->data().played_vedle = 0;
    }
    else if(g->mode == BANG)
    {
        vedle = g->game_order[pos]->play_vedle();
    }
    else if(g->mode == SLAB_BANG)
    {
        vedle = g->game_order[pos]->resolve_slab_bang();
    }

    if(!vedle)
    {
        bool hp = g->game_order[pos]->dec_hp(1);
        if(!hp)
        {
            g->killed(enemy_id);
        }
    }
    g->mode = NONE;
}

void GameTools::duel(Game *g)
{
    bool result = false;
    bool hp = false;
    int duel_turn = (g->duel_active_turn ? g->active_player :
                         GameTools::id_to_pos(g, g->game_order[g->active_player]->target_id));

    if(!g->game_order[duel_turn]->data().isai)
    {
        result = false;
    }
    else
    {
        result = g->game_order[duel_turn]->play_bang();
    }

    if(!result)
    {
        hp = g->game_order[duel_turn]->dec_hp(1);
        if(!hp)
        {
            int killed_id = g->game_order[duel_turn]->id;
            g->active_player = (duel_turn == g->active_player ? (g->active_player + 1) % g->player_alive
                                                              : g->active_player);
            g->killed(killed_id);
        }
        g->mode = NONE;
        g->duel_active_turn = false;
        return;
    }

    g->duel_active_turn = !g->duel_active_turn;
}
