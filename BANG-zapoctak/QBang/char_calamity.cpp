#include "game.h"
#include "ai.h"

int Calamity::game_phase()
{
    int result = Player::game_phase();
    if(!played_bang && Ai::index_name(cards_hand, VEDLE) != -1 && result == 0)
    {
        for(size_t i = 0; i < g->game_order.size(); i++)
        {
            if(enemies_id.find(g->game_order[i]->id) != enemies_id.end() &&
                g->distances.find(id)->second[g->game_order[i]->id] <= 1)
            {
                target_id = g->game_order[i]->id;
                g->deck.push_back(cards_hand[Ai::index_name(cards_hand, VEDLE)]);
                cards_hand.erase(cards_hand.begin() + Ai::index_name(cards_hand, VEDLE));
                played_bang = true;
                return 1;
            }
        }
    }
    return result;
}

bool Calamity::play_bang()
{
    bool res = Player::play_bang();

    if(!res)
    {
        if(Ai::index_name(cards_hand, VEDLE) != -1)
        {
            res = true;
            Ai::discard_card(g, cards_hand, cards_desk, Ai::index_name(cards_hand, VEDLE));
        }
    }

    return res;
}

bool Calamity::play_vedle()
{
    bool res = Player::play_vedle();

    if(!res)
    {
        if(Ai::index_name(cards_hand, BANG) != -1)
        {
            res = true;
            Ai::discard_card(g, cards_hand, cards_desk, Ai::index_name(cards_hand, BANG));
        }
    }

    return res;
}

bool Calamity::resolve_slab_bang()
{
    if(id != g->game_order[g->active_player]->id)
    {
        enemies_id.insert(g->game_order[g->active_player]->id);
        if(role == 'S')
        {
            Ai::vice_add_enemy(g, g->game_order[g->active_player]->id);
        }
    }


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

    int hand_vedle_bang = 0;
    for(size_t i = 0; i < cards_hand.size(); i++)
    {
        if(cards_hand[i].name == "Vedle" || cards_hand[i].name == "Bang")
        {
            hand_vedle_bang++;
        }
    }

    //potrebujeme odhodit 2x vedle
    if(vedle + hand_vedle_bang >= 2)
    {
        while(vedle < 2)
        {
            if(Ai::index_name(cards_hand, VEDLE) != -1)
            {
                Ai::discard_card(g, cards_hand, cards_desk, Ai::index_name(cards_hand, VEDLE));
            }
            else
            {
                Ai::discard_card(g, cards_hand, cards_desk, Ai::index_name(cards_hand, VEDLE));
            }
            vedle++;
        }
        return true;
    }
    return false;
}
