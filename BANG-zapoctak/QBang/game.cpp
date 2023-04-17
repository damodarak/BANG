#include <algorithm>
#include <fstream>
#include <string>

#include "game.h"
#include "ai.h"
#include "notaihandle.h"

using namespace std;

Card Game::draw_from_deck()
{
	Card c;
	c = deck.front();
	deck.pop_front();
	return c;
}
void Game::create(int players, string roles)
{
    GameTools::load_chars(this, characters);

    player_alive = player_count = players;
    create_players(players - 1);

    //vyber postavy podle ratingu postav
    int num = (characters[players]->data().ranking > characters[players + 1]->data().ranking ? players + 1 : players);

    Hrac p = move(characters[num]);
    characters.erase(characters.begin() + num);
    p->data().isai = false;
    game_order.push_back(move(p));

    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };

    int shuffle_count = 20;
    for (int i = 0; i < shuffle_count; i++)
    {
        shuffle(begin(roles), end(roles), rng);//pro nahodny vyber roli
    }

    for (int i = 0; i < players; i++)
    {
        game_order[i]->data().role = roles[i];
        if(roles[i] == 'S')
        {
            game_order[i]->health++;
            game_order[i]->max_health++;
        }
    }

}
void Game::create_players(int count)
{
    for (size_t i = 0; i < (size_t)count; i++)
    {
        Hrac p = move(characters[i + (characters[i]->data().ranking < characters[i + 1]->data().ranking ? 0 : 1)]);
        characters.erase(characters.begin() + i);
        game_order.push_back(move(p));
	}
}
void Game::draw_cards_start()
{
	for (size_t i = 0; i < game_order.size(); i++)
	{
        for (size_t j = 0; j < (size_t)game_order[i]->max_health; j++)
		{
            game_order[i]->data().cards_hand.push_back(draw_from_deck());
		}	
	}
}
bool Game::finished()
{
    //pocet hrac == 1, umrel serif, nezustali bandite a odpadlik
	if (player_alive == 1)
	{
		return true;
	}
    else if(game_order[0]->data().role != 'S')
    {
        return true;
    }
    else if(game_order[0]->health == 0)
    {
        return true;
    }
    else
    {
        for(size_t i = 0; i < game_order.size(); i++)
        {
            if(game_order[i]->data().role != 'S' && game_order[i]->data().role != 'V')
            {
                return false;
            }
        }
        return true;
    }
}
void Game::set_initial_enemies()
{
	int sheriff_id = 0;
	vector<int> ids;

	for (size_t i = 0; i < game_order.size(); i++)
	{
        if (game_order[i]->data().role == 'S')
		{
			sheriff_id = game_order[i]->id;
		}
		else
		{
			ids.push_back(game_order[i]->id);
		}
	}

	for (size_t i = 0; i < game_order.size(); i++)
	{
		game_order[i]->set_enemy(sheriff_id, ids);
	}
}
void Game::set_distances()
{
    distances.clear();

    //ZAKLADNI GRAF
	for (size_t i = 0; i < game_order.size(); i++)
	{
		map<int, int> m;
		for (size_t j = 0; j < game_order.size() / 2; j++)
		{		
            m[game_order[(i + j + 1) % player_alive]->id] = static_cast<int>(j) + 1;//forward
            m[game_order[(i - j + player_alive - 1) % player_alive]->id] = static_cast<int>(j) + 1;//backwards
		}

		distances[game_order[i]->id] = m;
	}

    //SCHOPNOSTI POSTAV
	for (size_t i = 0; i < game_order.size(); i++)
	{
        if ((Chars)game_order[i]->data().ranking == ROSE)
		{
			change_distance(game_order[i]->id, -1);
		}
        else if ((Chars)game_order[i]->data().ranking == PAUL)
		{
			int paul_id = game_order[i]->id;

			for (auto&& pl : distances)
			{
				change_distance(pl.first, 1, paul_id);
			}
		}
	}

    //ZBRANE
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(Ai::has_gun(game_order[i]->cards_desk) != -1)
        {
            change_distance(game_order[i]->id, -(Ai::has_gun(game_order[i]->cards_desk) - 1));
        }
    }

    //KONE
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(Ai::index_name(game_order[i]->cards_desk, APPALOOSA) != -1)
        {
            change_distance(game_order[i]->id, -1);
        }
    }
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(Ai::index_name(game_order[i]->cards_desk, MUSTANG) != -1)
        {
            int id = game_order[i]->id;

            for (auto&& pl : distances)
            {
                change_distance(pl.first, 1, id);
            }
        }
    }
}
void Game::change_distance(int id1, int change, int id2)//zmena hrany v orientovanem ohodnocenem grafu
{
	if (id1 == id2)
	{
		return;
	}

	if (id2 == -1)
	{
		for (auto&& pl : distances.find(id1)->second)
		{
			pl.second += change;
		}
	}
	else 
	{
		distances.find(id1)->second[id2] += change;
	}
}
void Game::game_loop()
{
    set_distances();
    GameTools::suzy_abil(this);

    //hraje AI a neni zadny mod zaply
    if(mode == NONE && game_order[active_player]->data().isai)
    {
        if(!game_order[active_player]->data().drawed)
        {
            game_order[active_player]->data().drawed = true;
            if(game_order[active_player]->resolve_dyn())
            {
                if(!game_order[active_player]->dec_hp(3))//neprezil to
                {
                    int dead = game_order[active_player]->id;
                    active_player = (active_player + 1) % player_alive;
                    killed(dead);
                    return;
                }
            }
            if(!game_order[active_player]->resolve_jail())
            {
                game_order[active_player]->turn_reset();
                active_player = (active_player + 1) % player_alive;
                return;
            }
            game_order[active_player]->draw_phase();
        }
        else
        {
            bool game = game_order[active_player]->game_phase();
            if(!game)
            {
                game_order[active_player]->turn_reset();
                game_order[active_player]->discard_phase();
                active_player = (active_player + 1) % player_alive;
            }
            if(mode == BANG && (Chars)game_order[active_player]->data().ranking == SLAB)
            {
                mode = SLAB_BANG;
            }
        }
    }
    else if(mode != NONE)
    {
        resolve_played_card();
    }
    //hraje notAI
    else if(!game_order[active_player]->data().isai)
    {
        NotaiHandle::resolve_notai_play(this);
    }
}
void Game::vulture_sam(std::vector<Card>& reward)
{
    //schopnost vulture_sama, pokud je nekdo vyrazen ze hry, tak ziska vsechny jeho karty
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if((Chars)game_order[i]->data().ranking == VULTURE && game_order[i]->health > 0)
        {
            for(size_t j = 0; j < reward.size(); j++)
            {
                game_order[i]->data().cards_hand.push_back(reward[j]);
            }
            return;
        }
    }

    for(size_t j = 0; j < reward.size(); j++)
    {
        deck.push_back(reward[j]);
    }
}
void Game::killed(int id)
{    
    //v pripade nedohrani kulometu a indianu se musi posunou rucicka na hrace o jednu dozadu
    if((mode == KULOMET || mode == INDIANI) && neu_turn != -1 && neu_turn != 0)
    {
        neu_turn = (neu_turn - 1 + player_alive) % player_alive;
    }

    int active_id = game_order[active_player]->id;

    int pos = GameTools::id_to_pos(this, id);
    char role = game_order[pos]->data().role;

    GameTools::discard_killed(this, role, pos);
    vector<Card> reward = game_order[pos]->give_all_cards();

    GameTools::rm_enemy(this, id);
    dead.push_back(move(game_order[pos]));
    game_order.erase(game_order.begin() + pos);
    player_alive--;

    vulture_sam(reward);
    set_distances();
    GameTools::set_notai(this);

    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == active_id)
        {
            active_player = i;
        }
    }

    //konec duelu
    duel_active_turn = false;
}
bool Game::can_respond_with_card(int index)
{
    Card c = ((size_t)index >= game_order[notai]->hand_size() ?
    game_order[notai]->cards_desk[index - game_order[notai]->hand_size()] :
                  game_order[notai]->data().cards_hand[index]);

    if(mode == DUEL || mode == INDIANI)
    {
        return c.mode == BANG || ((Chars)game_order[notai]->data().ranking == CALAMITY && c.mode == VEDLE);
    }
    else if(mode == KULOMET)
    {
        return c.mode == VEDLE || ((Chars)game_order[notai]->data().ranking == CALAMITY && c.mode == BANG) ||
                (c.mode == BAREL && game_order[notai]->data().barel == 0);
    }
    else if(mode == BANG || mode == VEDLE)
    {
        return c.mode == VEDLE || ((Chars)game_order[notai]->data().ranking == CALAMITY && c.mode == BANG) ||
                (c.mode == BAREL && game_order[notai]->data().barel == 0);
    }
    else if(mode == SLAB_BANG)
    {
        if(c.mode == VEDLE)
        {
            return true;
        }
        else if(game_order[notai]->data().barel == 0 && c.mode == BAREL)
        {
            return true;
        }
        else if(c.mode == BAREL && game_order[notai]->data().barel < 2)
        {
            return true;
        }
        else if((Chars)game_order[notai]->data().ranking == CALAMITY && c.mode == BANG)
        {
            return true;
        }
    }
    return false;
}
void Game::resolve_played_card()
{
    //Kulomet, Indiani...postupne kazdy musi odhodit potrebnou kartu nebo ekvivalent, jinak ztraci zivot
    if(mode == KULOMET)
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
            return;
        }

        bool vedle = false;
        if(!game_order[neu_turn]->data().isai)
        {
            vedle = false;
            game_order[neu_turn]->data().barel = 0;
        }
        else
        {
            vedle = game_order[neu_turn]->play_vedle();
        }
        int react = neu_turn;
        next_neu();

        if(!vedle)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[react]->id);
            }
        }
    }
    else if(mode == INDIANI)
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
            return;
        }

        bool bang = false;
        if(!game_order[neu_turn]->data().isai)
        {
            bang = false;
        }
        else
        {
            bang = game_order[neu_turn]->play_bang();
        }
        int react = neu_turn;
        next_neu();

        if(!bang)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[react]->id);
            }
        }
    }
    else if(mode == DUEL)
    {
        bool result;
        if(duel_active_turn)
        {
            if(!game_order[active_player]->data().isai)
            {
                result = false;
            }
            else
            {
                result = game_order[active_player]->play_bang();
            }

            if(!result)
            {
                bool hp = game_order[active_player]->dec_hp(1);
                if(!hp)
                {
                    int killed_id = game_order[active_player]->id;
                    active_player = (active_player + 1) % player_alive;
                    killed(killed_id);
                }
                mode = NONE;
                duel_active_turn = false;
                return;
            }
        }
        else
        {
            int enemy_id = game_order[active_player]->target_id;
            int pos = GameTools::id_to_pos(this, enemy_id);

            if(!game_order[pos]->data().isai)
            {
                result = false;
            }
            else
            {
                result = game_order[pos]->play_bang();
            }

            if(!result)
            {
                bool hp = game_order[pos]->dec_hp(1);
                if(!hp)
                {
                    killed(game_order[pos]->id);
                }
                mode = NONE;
                duel_active_turn = false;
                return;
            }
        }
        duel_active_turn = !duel_active_turn;
    }
    else if(mode == HOKYNARSTVI)
    {
        if(neu_turn == -1)
        {
            neu_turn = active_player;
            if(emporio.size() == 0)
            {
                GameTools::load_emporio(this);
            }
            return;
        }
        if(neu_turn != -1 && !game_order[neu_turn]->data().isai)
        {
            return;
        }
        int choice = game_order[neu_turn]->choose(emporio);
        game_order[neu_turn]->data().cards_hand.push_back(emporio[choice]);
        emporio.erase(emporio.begin() + choice);

        next_neu();
    }
    else if(mode == BANG || mode == VEDLE)
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = GameTools::id_to_pos(this, enemy_id);

        bool vedle;
        if(!game_order[pos]->data().isai)
        {
            vedle = false;
            game_order[pos]->data().barel = 0;
            game_order[pos]->data().played_vedle = 0;
        }
        else
        {
            vedle = game_order[pos]->play_vedle();
        }

        if(!vedle)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                killed(enemy_id);
            }
        }
        mode = NONE;
    }
    else if(mode == BALOU)
    {
        int pos = GameTools::id_to_pos(this, game_order[active_player]->target_id);
        Card c = game_order[pos]->give_random_card();
        deck.push_back(c);
        mode = NONE;
    }
    else if(mode == PANIKA)
    {
        int enemy_id = game_order[active_player]->target_id;

        if(Ai::can_play_panika(this, game_order[active_player]->id, enemy_id))
        {
            int pos = GameTools::id_to_pos(this, enemy_id);
            Card c = game_order[pos]->give_random_card();
            game_order[active_player]->data().cards_hand.push_back(c);
        }
        mode = NONE;
    }
    else if(mode == SLAB_BANG)
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = GameTools::id_to_pos(this, enemy_id);
        bool slab_vedle;

        if(!game_order[pos]->data().isai)
        {
            slab_vedle = false;
            game_order[pos]->data().barel = 0;
            game_order[pos]->data().played_vedle = 0;
        }
        else
        {
            slab_vedle = game_order[pos]->resolve_slab_bang();
        }

        if(!slab_vedle)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                killed(enemy_id);
            }
        }
        mode = NONE;
    }
}
void Game::resolve_notai_react(size_t c_index)
{
    Card c;
    //barel
    if(c_index >= game_order[notai]->hand_size())
    {
        c = game_order[notai]->cards_desk[Ai::index_name(game_order[notai]->cards_desk, BAREL)];
    }
    else
    {
        c = game_order[notai]->data().cards_hand[c_index];
        if((Modes)c.mode == BAREL)
        {
            return;
        }
        game_order[notai]->data().cards_hand.erase(game_order[notai]->data().cards_hand.begin() + c_index);
        deck.push_back(c);
    }

    if(mode == DUEL)
    {
        duel_active_turn = !duel_active_turn;
    }
    else if(mode == KULOMET)
    {
        if((Modes)c.mode == BAREL)
        {
            if(game_order[notai]->resolve_barrel())
            {
                game_order[notai]->data().barel = 0;
                next_neu();
            }
            else
            {
                game_order[notai]->data().barel++;
            }
        }
        else
        {
            next_neu();
        }
    }
    else if(mode == INDIANI)
    {
        next_neu();
    }
    else if(mode == SLAB_BANG)
    {
        if((Modes)c.mode == BAREL)
        {
            if(game_order[notai]->resolve_barrel() && game_order[notai]->data().played_vedle == 1)
            {
                game_order[notai]->data().barel = 0;
                game_order[notai]->data().played_vedle = 0;
                mode = NONE;
            }
            else
            {
                game_order[notai]->data().barel++;
            }
        }
        else
        {
            game_order[notai]->data().played_vedle++;
        }

        if(game_order[notai]->data().played_vedle == 2)
        {
            mode = NONE;
        }

    }
    else if(mode == VEDLE || mode == BANG)
    {
        if((Modes)c.mode == BAREL)
        {
            if(game_order[notai]->resolve_barrel())
            {
                game_order[notai]->data().barel = 0;
                mode = NONE;
            }
            else
            {
                game_order[notai]->data().barel++;
            }
        }
        else
        {
            mode = NONE;
        }
    }
}
void Game::next_neu()
{
    neu_turn = (neu_turn + 1) % player_alive;
    if(active_player == neu_turn)
    {
        mode = NONE;
        neu_turn = -1;
    }
}
