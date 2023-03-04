#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>

#include "player.h"
#include "characters/characters.h"

class Game {
public:
	Game() : active_player_id(0), player_count(0), player_alive(0) {};
	void next_player_move();
	void add_player();
	void draw_roles();
	void load_characters();
	void load_cards();
	Card draw_from_deck();
	void draw_cards();
private:
	std::vector<Player*> game_order;
	std::map<int, int> distances;
	int active_player_id;
	int player_count;
	int player_alive;
	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Player*> characters;
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
};

#endif