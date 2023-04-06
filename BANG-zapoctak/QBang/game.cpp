#include <algorithm>
#include <fstream>
#include <string>

#include "game.h"
#include "ai.h"

using namespace std;

void Game::load_characters()
{
    //nacteni vsech moznych hracu
	Hrac pl = make_unique<Bart>(this);
	characters.push_back(move(pl));
	pl = make_unique<Blackj>(this);
	characters.push_back(move(pl));
	pl = make_unique<Calamity>(this);
	characters.push_back(move(pl));
	pl = make_unique<Carlson>(this);
	characters.push_back(move(pl));
	pl = make_unique<Cringo>(this);
	characters.push_back(move(pl));
	pl = make_unique<Jesse>(this);
	characters.push_back(move(pl));
    pl = make_unique<Jourd>(this);
    characters.push_back(move(pl));
    pl = make_unique<Ketchum>(this);
    characters.push_back(move(pl));
    pl = make_unique<Lucky>(this);
    characters.push_back(move(pl));
	pl = make_unique<Paul>(this);
	characters.push_back(move(pl));
	pl = make_unique<Pedro>(this);
	characters.push_back(move(pl));
	pl = make_unique<Rose>(this);
	characters.push_back(move(pl));
	pl = make_unique<Slab>(this);
	characters.push_back(move(pl));
	pl = make_unique<Suzy>(this);
	characters.push_back(move(pl));
	pl = make_unique<Vulture>(this);
	characters.push_back(move(pl));
	pl = make_unique<Willy>(this);
	characters.push_back(move(pl));

	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
	for (size_t i = 0; i < 20; i++)
	{
		shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav
	}
}
void Game::load_card(vector<string>& v)
{
    Card card(stoi(v[4]), v[0], v[3][0], v[1], stoi(v[2]), v[5], stoi(v[6]));

    deck.push_back(card);

    //po kazde nove karte premichame balicek
	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
    shuffle(begin(deck), end(deck), rng);//pro nahodny vyber karet
}
Card Game::draw_from_deck()
{
	Card c;
	c = deck.front();
	deck.pop_front();
	return c;
}
void Game::create(int players, string roles)
{
    player_alive = player_count = players;
    create_players(players - 1);

    //vyber postavy podle ratingu postav
    int num = (characters[players]->ranking > characters[players + 1]->ranking ? players + 1 : players);

    Hrac p = move(characters[num]);
    characters.erase(characters.begin() + num);
    p->isai = false;
    game_order.push_back(move(p));

    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };
    for (int i = 0; i < 20; i++)
    {
        shuffle(begin(roles), end(roles), rng);//pro nahodny vyber roli
    }

    for (int i = 0; i < players; i++)
    {
        game_order[i]->role = roles[i];
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
		if (characters[i]->ranking < characters[i + 1]->ranking)
		{
			Hrac p = move(characters[i]);
			characters.erase(characters.begin() + i);
			game_order.push_back(move(p));
		}
		else
		{
			Hrac p = move(characters[i + 1]);
			characters.erase(characters.begin() + i + 1);
			game_order.push_back(move(p));
		}
	}
}
void Game::rotate_serif()
{
    while (game_order[0]->role != 'S')
	{
		rotate(game_order.begin(), game_order.begin() + 1, game_order.end());
	}

    active_player = 0;
}
void Game::draw_cards_start()
{
	Card c;
	for (size_t i = 0; i < game_order.size(); i++)
	{
        for (size_t j = 0; j < (size_t)game_order[i]->max_health; j++)
		{
			c = draw_from_deck();
            game_order[i]->cards_hand.push_back(c);
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
    else if(game_order[0]->role != 'S')
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
            if(game_order[i]->role != 'S' && game_order[i]->role != 'V')
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
        if (game_order[i]->role == 'S')
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
        if (game_order[i]->name == "rose")
		{
			change_distance(game_order[i]->id, -1);
		}
        else if (game_order[i]->name == "paul")
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
        if(Ai::index(game_order[i]->cards_desk, "Appaloosa") != -1)
        {
            change_distance(game_order[i]->id, -1);
        }
    }
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(Ai::index(game_order[i]->cards_desk, "Mustang") != -1)
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
void Game::saloon()
{
    //vsem +1 zivot, pokud jiz nemaji max. pocet
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->max_health > game_order[i]->health)
        {
            game_order[i]->health++;
        }
    }
}
void Game::game_loop()
{
    set_distances();

    //SPECIAL CASE FOR SUZY
    if(game_order[active_player]->name == "suzy" &&
            game_order[active_player]->cards_hand.size() == 0)
    {
        game_order[active_player]->ability();
        return;
    }

    //hraje AI a neni zadny mod zaply
    if(mode == "" && game_order[active_player]->isai)
    {
        if(!game_order[active_player]->drawed)
        {
            game_order[active_player]->drawed = true;
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
            int game = game_order[active_player]->game_phase();
            if(game == 0)
            {
                game_order[active_player]->turn_reset();
                game_order[active_player]->discard_phase();
                active_player = (active_player + 1) % player_alive;
            }
            else if(game == 1)
            {
                mode = deck.back().name;
                if(mode == "Bang" && game_order[active_player]->name == "slab")
                {
                    mode = "Slab";
                }
            }
            //game == 2 => return
        }
    }
    else if(mode != "")
    {
        resolve_played_card();
    }
    //hraje notAI
    else if(!game_order[active_player]->isai)
    {
        resolve_notai_play();
    }
}
string Game::id_to_name(int id)
{
    if(id == -1)
    {
        return "";
    }
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == id)
        {
            return game_order[i]->name;
        }
    }
    return "";
}
void Game::vulture_sam(std::vector<Card>& reward)
{
    //schopnost vulture_sama, pokud je nekdo vyrazen ze hry, tak ziska vsechny jeho karty
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->name == "vulture" && game_order[i]->health > 0)
        {
            for(size_t j = 0; j < reward.size(); j++)
            {
                game_order[i]->cards_hand.push_back(reward[j]);
            }
            return;
        }

    }

    for(size_t j = 0; j < reward.size(); j++)
    {
        deck.push_back(reward[j]);
    }
}
void Game::resolve_played_card()
{
    //Kulomet, Indiani...postupne kazdy musi odhodit potrebnou kartu nebo ekvivalent, jinak ztraci zivot
    if(mode == "Kulomet")
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
            return;
        }

        bool vedle = false;
        if(!game_order[neu_turn]->isai)
        {
            vedle = false;
            game_order[neu_turn]->barel = 0;
        }
        else
        {
            vedle = game_order[neu_turn]->play_vedle();
        }
        int react = neu_turn;
        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }

        if(!vedle)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[react]->id);
            }
        }
    }
    else if(mode == "Indiani")
    {
        if(neu_turn == -1)
        {
            neu_turn = (active_player + 1) % player_alive;
            return;
        }

        bool bang = false;
        if(!game_order[neu_turn]->isai)
        {
            bang = false;
        }
        else
        {
            bang = game_order[neu_turn]->play_bang();
        }
        int react = neu_turn;
        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }

        if(!bang)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[react]->id);
            }
        }
    }
    else if(mode == "Duel")
    {
        bool result;
        if(duel_active_turn)
        {
            if(!game_order[active_player]->isai)
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
                mode = "";
                duel_active_turn = false;
                return;
            }
        }
        else
        {
            int enemy_id = game_order[active_player]->target_id;
            int pos = id_to_pos(enemy_id);

            if(!game_order[pos]->isai)
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
                mode = "";
                duel_active_turn = false;
                return;
            }
        }
        duel_active_turn = !duel_active_turn;
    }
    else if(mode == "Hokynarstvi")
    {
        if(neu_turn == -1)
        {
            neu_turn = active_player;
            load_emporio();
            return;
        }
        if(neu_turn != -1 && !game_order[neu_turn]->isai)
        {
            return;
        }
        int choice = game_order[neu_turn]->choose(emporio);
        game_order[neu_turn]->cards_hand.push_back(emporio[choice]);
        emporio.erase(emporio.begin() + choice);

        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Bang" || mode == "Vedle")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);

        bool vedle;
        if(!game_order[pos]->isai)
        {
            vedle = false;
            game_order[pos]->barel = 0;
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
        mode = "";
    }
    else if(mode == "CatBalou")
    {
        int pos = id_to_pos(game_order[active_player]->target_id);
        Card c = game_order[pos]->give_random_card();
        deck.push_back(c);
        mode = "";
    }
    else if(mode == "Panika")
    {
        int enemy_id = game_order[active_player]->target_id;

        if(Ai::can_play_panika(this, game_order[active_player]->id, enemy_id))
        {
            int pos = id_to_pos(enemy_id);
            Card c = game_order[pos]->give_random_card();
            game_order[active_player]->cards_hand.push_back(c);
        }
        mode = "";
    }
    else if(mode == "Salon")
    {
        saloon();
        mode = "";
    }
    else if(mode == "Slab")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);
        bool slab_vedle;

        if(!game_order[pos]->isai)
        {
            slab_vedle = false;
            game_order[pos]->barel = 0;
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
        mode = "";
    }
}
int Game::id_to_pos(int id)
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == id)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}
void Game::load_emporio()
{
    //byla zahrana karta Hokynarstvi
    for(size_t i = 0; i < game_order.size(); i++)
    {
        emporio.push_back(draw_from_deck());
    }
}
void Game::killed(int id)
{
    //konec duelu
    duel_active_turn = false;

    //v pripade nedohrani kulometu a indianu se musi posunou rucicka na hrace o jednu dozadu
    if((mode == "Kulomet" || mode == "Indiani") && neu_turn != -1)
    {
        neu_turn = (neu_turn - 1 + player_alive) % player_alive;
    }

    int active_id = game_order[active_player]->id;

    int pos = id_to_pos(id);
    char role = game_order[pos]->role;
    //nekdo zabil banditu
    if(role == 'B' && deck.back().name != "Dynamit")
    {
        for(int i = 0; i < 3; i++)
        {
            game_order[active_player]->cards_hand.push_back(draw_from_deck());
        }
    }
    //Serif zabil sveho pomocnika
    else if(role == 'V' && game_order[active_player]->role == 'S' && deck.back().name != "Dynamit")
    {
        vector<Card> shame = game_order[active_player]->give_all_cards();
        for(size_t i = 0; i < shame.size(); i++)
        {
            deck.push_back(shame[i]);
        }
    }
    vector<Card> reward = game_order[pos]->give_all_cards();

    rm_enemy(id);
    dead.push_back(move(game_order[pos]));
    game_order.erase(game_order.begin() + pos);
    player_alive--;

    vulture_sam(reward);
    set_distances();
    set_notai();

    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(game_order[i]->id == active_id)
        {
            active_player = i;
        }
    }
}
void Game::set_notai()
{
    for(size_t i = 0; i < game_order.size(); i++)
    {
        if(!game_order[i]->isai)
        {
            notai = i;
            return;
        }
    }
    notai = 0;
}
void Game::resolve_notai_play()
{
    int pos = id_to_pos(game_order[active_player]->target_id);
    int enemy = game_order[active_player]->target_id;

    //zbran
    if(deck.back().range != 0)
    {
        if(Ai::has_gun(game_order[active_player]->cards_desk) == -1)
        {
            Card c = deck.back();
            deck.pop_back();
            game_order[active_player]->cards_desk.push_back(c);
            if(c.name == "Volcanic")
            {
                game_order[active_player]->played_bang = false;
            }
        }
    }
    //vezeni
    else if(deck.back().name == "Vezeni")
    {
        if(pos != -1 && game_order[pos]->role != 'S'
                && Ai::index(game_order[pos]->cards_desk, "Vezeni") == -1)
        {
            Card c = deck.back();
            deck.pop_back();
            game_order[pos]->cards_desk.push_back(c);
            game_order[pos]->enemies_id.insert(game_order[active_player]->id);
        }
    }
    //ostatni modre karty
    else if(deck.back().edge == 'M')
    {
        if(Ai::index(game_order[active_player]->cards_desk, deck.back().name) == -1)
        {
            Card c = deck.back();
            deck.pop_back();
            game_order[active_player]->cards_desk.push_back(c);
        }
    }
    //dostavnik
    else if(deck.back().name == "Dostavnik")
    {
        game_order[active_player]->dostavnik_wells(2);
    }
    //wellsfargo
    else if(deck.back().name == "WellsFargo")
    {
        game_order[active_player]->dostavnik_wells(3);
    }
    //pivo
    else if(deck.back().name == "Pivo")
    {
        if(game_order[active_player]->health < game_order[active_player]->max_health)
        {
            game_order[active_player]->health++;
        }
    }
    //neu
    else if(deck.back().card_type == "neu")
    {
        if(deck.back().name == "Salon")
        {
            saloon();
        }
        else
        {
            mode = deck.back().name;
            if(mode == "Hokynarstvi")
            {
                neu_turn = active_player;
                load_emporio();
            }
            else
            {
                neu_turn = (active_player + 1) % player_alive;
            }
        }
    }
    //Cat Balou
    else if(deck.back().name == "CatBalou")
    {
        if(pos != -1 && Ai::panika_balou_play(this, enemy))
        {
            Card c = game_order[pos]->give_random_card();
            deck.push_back(c);
        }
    }
    //Panika
    else if(deck.back().name == "Panika")
    {
        if(pos != -1 && Ai::panika_balou_play(this, enemy) &&
            Ai::can_play_panika(this, game_order[active_player]->id, enemy))
        {
            Card c = game_order[pos]->give_random_card();
            game_order[active_player]->cards_hand.push_back(c);
        }
    }
    //bang
    else if((deck.back().name == "Bang" ||
             (deck.back().name == "Vedle" && game_order[active_player]->name == "calamity")) &&
            !game_order[active_player]->played_bang &&
            distances.find(game_order[active_player]->id)->second[enemy] <= 1)
    {
        if(game_order[active_player]->name == "willy" || Ai::has_gun(game_order[active_player]->cards_desk) == 1)
        {
            game_order[active_player]->played_bang = false;
        }
        else
        {
            game_order[active_player]->played_bang = true;
        }
        mode = (game_order[active_player]->name == "slab") ? "Slab" : "Bang";
    }
    else if(deck.back().name == "Duel")
    {
        mode = deck.back().name;
        duel_active_turn = false;
    }

    set_distances();
}
void Game::ai_react()
{
    // podminky jsou samopopisujici
    if(mode == "Duel")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);
        bool result = game_order[pos]->play_bang();

        if(!result)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[pos]->id);
            }
            mode = "";
            duel_active_turn = false;
            return;
        }
        else
        {
            duel_active_turn = !duel_active_turn;
        }
    }
    else if(mode == "Bang" || mode == "Vedle")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);
        bool vedle = game_order[pos]->play_vedle();
        if(!vedle)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                killed(enemy_id);
            }
        }
        mode = "";
    }
    else if(mode == "Slab")
    {
        int enemy_id = game_order[active_player]->target_id;
        int pos = id_to_pos(enemy_id);
        bool slab_vedle = game_order[pos]->resolve_slab_bang();
        if(!slab_vedle)
        {
            bool hp = game_order[pos]->dec_hp(1);
            if(!hp)
            {
                killed(enemy_id);
            }
        }
        mode = "";
    }
    else if(mode == "Hokynarstvi")
    {
        int choice = game_order[neu_turn]->choose(emporio);
        game_order[neu_turn]->cards_hand.push_back(emporio[choice]);
        emporio.erase(emporio.begin() + choice);

        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Indiani")
    {
        bool bang = game_order[neu_turn]->play_bang();
        int react = neu_turn;
        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }

        if(!bang)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {
                killed(game_order[react]->id);
            }
        }
    }
    else if(mode == "Kulomet")
    {
        bool vedle = game_order[neu_turn]->play_vedle();
        int react = neu_turn;
        neu_turn = (neu_turn + 1) % player_alive;
        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }

        if(!vedle)
        {
            bool hp = game_order[react]->dec_hp(1);
            if(!hp)
            {

                killed(game_order[react]->id);
            }
        }     
    }
}
bool Game::notai_duel_react()
{
    //zda-li je na rade notAI v odehrani Bang pri duelu

    if(mode != "Duel")
    {
        return false;
    }


    return (game_order[active_player]->isai &&
            !duel_active_turn && !game_order[id_to_pos(game_order[active_player]->target_id)]->isai) ||
            (!game_order[active_player]->isai && duel_active_turn);
}
bool Game::can_respond_with_card(string name)
{
    if(mode == "Duel" || mode == "Indiani")
    {
        return name == "Bang" || (game_order[notai]->name == "calamity" && name == "Vedle");
    }
    else if(mode == "Kulomet")
    {
        return name == "Vedle" || (game_order[notai]->name == "calamity" && name == "Bang") ||
                (name == "Barel" && game_order[notai]->barel == 0);
    }
    else if(mode == "Bang" || mode == "Vedle")
    {
        return name == "Vedle" || (game_order[notai]->name == "calamity" && name == "Bang") ||
                (name == "Barel" && game_order[notai]->barel == 0);
    }
    else if(mode == "Slab")
    {
        if(name == "Vedle")
        {
            return true;
        }
        else if(game_order[notai]->barel == 0 && name == "Barel")
        {
            return true;
        }
        else if(name == "Barel" && game_order[notai]->barel < 2)
        {
            return true;
        }
        else if(game_order[notai]->name == "calamity" && name == "Bang")
        {
            return true;
        }
    }
    return false;
}
void Game::resolve_notai_react(size_t c_index)
{
    Card c;
    //barel
    if(c_index >= game_order[notai]->cards_hand.size())
    {
        c = game_order[notai]->cards_desk[Ai::index(game_order[notai]->cards_desk, "Barel")];
    }
    else
    {
        c = game_order[notai]->cards_hand[c_index];
        if(c.name == "Barel")
        {
            return;
        }
        game_order[notai]->cards_hand.erase(game_order[notai]->cards_hand.begin() + c_index);
        deck.push_back(c);
    }

    if(mode == "Duel")
    {
        duel_active_turn = !duel_active_turn;
    }
    else if(mode == "Kulomet")
    {
        if(c.name == "Barel")
        {
            if(game_order[notai]->resolve_barrel())
            {
                game_order[notai]->barel = 0;
                neu_turn = (neu_turn + 1) % player_alive;

                if(active_player == neu_turn)
                {
                    mode = "";
                    neu_turn = -1;
                }
            }
            else
            {
                game_order[notai]->barel++;
            }
        }
        else
        {
            neu_turn = (neu_turn + 1) % player_alive;

            if(active_player == neu_turn)
            {
                mode = "";
                neu_turn = -1;
            }
        }
    }
    else if(mode == "Indiani")
    {
        neu_turn = (neu_turn + 1) % player_alive;

        if(active_player == neu_turn)
        {
            mode = "";
            neu_turn = -1;
        }
    }
    else if(mode == "Slab")
    {
        if(c.name == "Barel")
        {
            if(game_order[notai]->resolve_barrel() && game_order[notai]->played_vedle == 1)
            {
                game_order[notai]->barel = 0;
                game_order[notai]->played_vedle = 0;
                mode = "";
            }
            else
            {
                game_order[notai]->barel++;
            }
        }
        else
        {
            game_order[notai]->played_vedle++;
        }

        if(game_order[notai]->played_vedle == 2)
        {
            mode = "";
        }

    }
    else if(mode == "Vedle" || mode == "Bang")
    {
        if(c.name == "Barel")
        {
            if(game_order[notai]->resolve_barrel())
            {
                game_order[notai]->barel = 0;
                mode = "";
            }
            else
            {
                game_order[notai]->barel++;
            }
        }
        else
        {
            mode = "";
        }
    }
}
void Game::rm_enemy(int id)
{
    //kdykoli nekdo umre, tak nema smysl uchovavat tohoto hrace v seznamu nepratel

    for(size_t i = 0; i <game_order.size(); i++)
    {
        if(game_order[i]->enemies_id.find(id) != game_order[i]->enemies_id.end())
        {
            game_order[i]->enemies_id.erase(id);
        }
    }
}
