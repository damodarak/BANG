#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <map>

#include "player.h"

class Game {
public:
	Game() : active_player_id(0), player_count(0), player_alive(0) {};
	void next_player_move();
	void add_player();
	void draw_roles();
	void draw_characters();
	void add_player()
	{
		Player pl;
	}
private:
	std::vector<Player> game_order;
	std::map<int, int> distances;
	int active_player_id;
	int player_count;
	int player_alive;
};

#endif