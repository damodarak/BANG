#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <memory>

#include "player.h"
#include "characters/characters.h"

typedef std::unique_ptr<Player> Hrac;

class Game {
public:
	Game() : active_player_id(0), player_count(0), player_alive(0) {};
	void load_characters();
	void load_cards();
	Card draw_from_deck();
	void draw_cards_start();
	void create(int players);
	void create_players(int count);
	void rotate_serif();
	bool finished();
	void set_initial_enemies();
	void set_distances();
	void change_distance(int id1, int change, int id2 = -1);

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
private:
	std::vector<Hrac> game_order;
	std::map<int, std::map<int, int>> distances;
	int active_player_id;
	
	std::vector<Hrac> characters;

	std::vector<std::string> convert_line(const std::string& line);
};

#endif