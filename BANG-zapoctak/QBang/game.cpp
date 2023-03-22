#include <algorithm>
#include <random>
#include <fstream>
#include <string>

#include "game.h"

using namespace std;

void Game::load_characters()
{
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
    Card card(stoi(v[4]), v[0], v[3][0], v[1], stoi(v[2]), v[5]);

    deck.push_back(card);

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

    int num = (characters[players]->ranking > characters[players + 1]->ranking ? players + 1 : players);

    Hrac p = move(characters[num]);
    characters.erase(characters.begin() + num);
    p->isai = false;
    game_order.push_back(move(p));

    auto rd = random_device{};
    auto rng = default_random_engine{ rd() };
    for (size_t i = 0; i < 20; i++)
    {
        shuffle(begin(roles), end(roles), rng);//pro nahodny vyber roli
    }

    for (size_t i = 0; i < players; i++)
    {
        game_order[i]->set_role(roles[i]);
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
	while (game_order[0]->say_role() != 'S')
	{
		rotate(game_order.begin(), game_order.begin() + 1, game_order.end());
	}

	active_player_id = game_order[0]->id;
}
void Game::draw_cards_start()
{
	Card c;
	for (size_t i = 0; i < game_order.size(); i++)
	{
        for (size_t j = 0; j < (size_t)game_order[i]->max_healt; j++)
		{
			c = draw_from_deck();
			game_order[i]->take_card(c);
		}	
	}
}
bool Game::finished()
{
	if (player_alive == 1)
	{
		return true;
	}
	else
	{
		for (size_t i = 0; i < game_order.size(); i++)
		{
			if (game_order[i]->say_role() != 'S')
			{
				continue;
			}
			else
			{
				return (game_order[i]->health == 0 ? true : false);
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
		if (game_order[i]->say_role() == 'S')
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
	for (size_t i = 0; i < game_order.size(); i++)
	{
		map<int, int> m;
		for (size_t j = 0; j < game_order.size() / 2; j++)
		{		
			m[game_order[(i + j + 1) % player_count]->id] = static_cast<int>(j) + 1;//forward
			m[game_order[(i - j + player_count - 1) % player_count]->id] = static_cast<int>(j) + 1;//backwards
		}

		distances[game_order[i]->id] = m;
	}

	for (size_t i = 0; i < game_order.size(); i++)
	{
		if (game_order[i]->name == "ROSE DOOLAN")
		{
			change_distance(game_order[i]->id, -1);
		}
		else if (game_order[i]->name == "PAUL REGRET")
		{
			int paul_id = game_order[i]->id;

			for (auto&& pl : distances)
			{
				change_distance(pl.first, 1, paul_id);
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
void Game::clear()
{
    deck.clear();
    emporio.clear();
    characters.clear();
    game_order.clear();
    player_count = player_alive = active_player_id = 0;
    distances.clear();
}

void Game::add_labels(QVector<QList<QLabel*>>& layout)
{
    size_t notai = 0;
    size_t count = layout.size();
    for(; notai < game_order.size(); notai++)
    {
        if(!game_order[notai]->isai)
        {
            break;
        }
    }
    game_order[notai]->char_l = layout[count-1][0];
    game_order[notai]->hp_l = layout[count-1][1];
    game_order[notai]->role_l = layout[count-1][2];
    for(int i = 3; i < 9; i++)
    {
        game_order[notai]->m_l.append(layout[count-1][i]);
    }
    for(int i = 9; i < 19; i++)
    {
        game_order[notai]->cards_l.append(layout[count-1][i]);
    }

    for(size_t i = 1; i <count; i++)
    {
        //setAILabels
    }
}
