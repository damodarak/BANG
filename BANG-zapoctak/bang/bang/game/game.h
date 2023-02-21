#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <map>

#include "player.h"

class Game {
public:
	void next_player_move();
	void add_player();
	void draw_roles();
	void draw_characters();
private:
	std::vector<Player> game_order;
	std::map<int, int> distances;
	int active_player;
};

#endif