#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

enum Action { DRAW, PlAY, DISCARD, PLAY_MISS, PLAY_2MISS, PLAY_BANG, DRAW_JAIL, DRAW_DYN, DRAW_EMPORIO  };

class Game;

static int next_player_id = 0;

class Player {
public:
	Player(int rank, int max_hp, std::string char_name) : ranking(rank), id(++next_player_id), 
		health(max_hp), max_healt(max_hp), played_bang(false), name(char_name) {};
	virtual void draw_phase(Game* g);
	virtual void game_phase() = 0;
	virtual void discard_phase() = 0;


	bool isai;
	int ranking;//for AI to choose beter character
protected:
	virtual bool resolve_jail(Game* g);
	virtual bool resolve_dyn(Game* g);

	int id;
	int health;
	int max_healt;
	bool played_bang;
	std::string name;
	std::vector<Card> cards;
	std::vector<Action> actions;

};

#endif