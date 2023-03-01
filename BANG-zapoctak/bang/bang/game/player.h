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
	Player() : id_(++next_player_id), played_bang(false) {}; 
	virtual void draw_phase();
	virtual void game_phase() = 0;
	virtual void discard_phase() = 0;
	void choose_character();
	void resolve_action(Game* g);
protected:
	int id_;
	int health;
	bool played_bang;
	std::vector<Card> cards;
	std::vector<Action> actions;

};

#endif