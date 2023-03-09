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
void Game::load_cards()
{
	ifstream f;
	f.open("../../../../bang/hraci_karty.txt");
	string line;
	getline(f, line);
	while (getline(f, line))
	{
		vector<string> v = convert_line(line);

		Card c;
		Card card(stoi(v[0]), v[1], v[2][0], v[3], stoi(v[4]), v[5]);
		c = card;
		
		deck.push_back(c);
	}

	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
	for (size_t i = 0; i < 20; i++)
	{
		shuffle(begin(deck), end(deck), rng);//pro nahodny vyber karet
	}
}
vector<string> Game::convert_line(const string& line)
{
	size_t found1 = line.find(",");
	string name = line.substr(0, found1);
	size_t found2 = line.find(",", found1 + 1);
	string suit = line.substr(found1 + 1, found2 - found1 - 1);
	size_t found3 = line.find(",", found2 + 1);
	string barva = line.substr(found3 + 1, 1);
	string rank = line.substr(found2 + 1, found3 - found2 - 1);
	size_t found4 = line.find(",", found3 + 1);
	size_t found5 = line.find(",", found4 + 1);
	string id = line.substr(found4 + 1, found5 - found4 - 1);
	string type = line.substr(line.size() - 3, 3);

	vector<string> v = {id, name, barva, suit, rank, type};
	return v;
}
Card Game::draw_from_deck()
{
	Card c;
	c = deck.front();
	deck.pop_front();
	return c;
}
void Game::create(int players)
{
	player_alive = player_count = players;

	ifstream f;
	f.open("../../../../bang/role.txt");
	string line;
	for (size_t i = 0; i < 4; i++)
	{
		getline(f, line);
		if (line[0] - '0' == players)
		{
			create_players(players - 1);

			cout << "1-" << characters[players]->name << endl;
			cout << "2-" << characters[players + 1]->name << endl;
			cout << "choose: ";
			string choice;
			cin >> choice;
			int num = stoi(choice);

			Hrac p = move(characters[players + num - 1]);
			characters.erase(characters.begin() + players + num - 1);
			p->isai = false;
			game_order.push_back(move(p));

			string roles = line.substr(2);
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

			f.close();
			return;
		}
	}
}
void Game::create_players(int count)
{
	for (size_t i = 0; i < count; i++)
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
		for (size_t j = 0; j < game_order[i]->max_healt; j++)
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

	for (size_t i = 0; i < game_order.size(); i++)
	{
		if (game_order[i]->say_role() == 'S')
		{
			sheriff_id = game_order[i]->id;
		}
	}

	for (size_t i = 0; i < game_order.size(); i++)
	{
		game_order[i]->set_enemy(sheriff_id);
	}
}
void Game::set_distances()//todo
{
	for (int i = 0; i < 4; i++)
	{
		cout << game_order[i]->id << ",";
	}

	for (size_t i = 0; i < game_order.size(); i++)
	{
		map<int, int> m;
		for (size_t j = 0; j < game_order.size() / 2; j++)
		{		
			m[game_order[(i + j + 1) % player_count]->id] = j + 1;//forward
			m[game_order[(i - j + 3) % player_count]->id] = j + 1;//backwards
		}

		distances[game_order[i]->id] = m;
	}

	//loop for rosa/paul
}