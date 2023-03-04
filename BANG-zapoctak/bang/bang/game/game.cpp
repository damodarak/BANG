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

	auto rd = std::random_device{};
	auto rng = default_random_engine{ rd() };
	shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav
	shuffle(begin(characters), end(characters), rng);
	shuffle(begin(characters), end(characters), rng);
}
void Game::load_cards()//todo
{
	ifstream f;
	f.open("hraci_karty.txt");
	string line;
	getline(f, line);
	while (getline(f, line))
	{
		size_t found1 = line.find(",");
		string name = line.substr(0, found1);
		size_t found2 = line.find(",", found1 + 1);
		string suit = line.substr(found1 + 1, found2 - found1 - 1);
		size_t found3 = line.find(",", found2 + 1);
		string cis = line.substr(found2 + 1, found3 - found2 - 1);
		int rank = stoi(cis);
		char barva = line[found3 + 1];
		size_t found4 = line.find(",", found3 + 1);
		string s = line.substr(found4 + 1);
		int id = stoi(s);

		Card c(id, name, barva, suit, rank);
		deck.push_back(c);
	}
	f.close();

	auto rd = std::random_device{};
	auto rng = default_random_engine{ rd() };
	shuffle(begin(deck), end(deck), rng);//pro nahodny vyber karet
	shuffle(begin(deck), end(deck), rng);
	shuffle(begin(deck), end(deck), rng);
}
Card Game::draw_from_deck()
{
	Card c;
	c = deck.front();
	deck.pop_front();
	return c;
}