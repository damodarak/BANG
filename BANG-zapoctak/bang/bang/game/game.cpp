#include <algorithm>
#include <random>
#include <fstream>
#include <string>

#include "game.h"

using namespace std;

void Game::load_characters()
{
	Player* pl = new Bart(this);
	characters.push_back(pl);
	pl = new Blackj(this);
	characters.push_back(pl);
	pl = new Calamity(this);
	characters.push_back(pl);
	pl = new Carlson(this);
	characters.push_back(pl);
	pl = new Cringo(this);
	characters.push_back(pl);
	pl = new Jesse(this);
	characters.push_back(pl);
	pl = new Jourd(this);
	characters.push_back(pl);
	pl = new Ketchum(this);
	characters.push_back(pl);
	pl = new Lucky(this);
	characters.push_back(pl);
	pl = new Paul(this);
	characters.push_back(pl);
	pl = new Pedro(this);
	characters.push_back(pl);
	pl = new Rose(this);
	characters.push_back(pl);
	pl = new Slab(this);
	characters.push_back(pl);
	pl = new Suzy(this);
	characters.push_back(pl);
	pl = new Vulture(this);
	characters.push_back(pl);
	pl = new Willy(this);
	characters.push_back(pl);

	auto rd = random_device{};
	auto rng = default_random_engine{ rd() };
	shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav
	shuffle(begin(characters), end(characters), rng);
	shuffle(begin(characters), end(characters), rng);
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
	shuffle(begin(deck), end(deck), rng);//pro nahodny vyber karet
	shuffle(begin(deck), end(deck), rng);
	shuffle(begin(deck), end(deck), rng);
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
void Game::start(int players)
{
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

			Player* p = characters[players + num - 1];
			characters.erase(characters.begin() + players + num - 1);
			p->isai = false;
			game_order.push_back(p);

			string roles = line.substr(2);
			auto rd = random_device{};
			auto rng = default_random_engine{ rd() };
			shuffle(begin(roles), end(roles), rng);//pro nahodny vyber roli
			shuffle(begin(roles), end(roles), rng);

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
			Player* p = characters[i];
			characters.erase(characters.begin() + i);
			p->isai = true;
			game_order.push_back(p);
		}
		else
		{
			Player* p = characters[i + 1];
			characters.erase(characters.begin() + i + 1);
			p->isai = true;
			game_order.push_back(p);
		}
	}
}
void Game::rotate_serif()
{
	while (game_order[0]->say_role() != 'S')
	{
		rotate(game_order.begin(), game_order.begin() + 1, game_order.end());
	}
}