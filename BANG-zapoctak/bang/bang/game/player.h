#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "card.h"

enum Action { DRAW, PlAY, DISCARD, DISCARD_RANDOM_CARD, PLAY_MISS, PLAY_2MISS, PLAY_BANG, DRAW_JAIL, DRAW_DYN, DRAW_EMPORIO  };

class Game;

static int next_player_id = 0;

class Player {
public:
	Player(int rank, int max_hp, std::string char_name, Game* game) : ranking(rank), id(++next_player_id), 
		health(max_hp), max_healt(max_hp), played_bang(false), name(char_name), g(game) {};
	virtual void draw_phase();
	virtual void game_phase() = 0;
	virtual void discard_phase() = 0;


	bool isai;
	int ranking;//for AI to choose beter character
protected:
	virtual bool resolve_jail();
	virtual bool resolve_dyn();

	int id;
	int health;
	int max_healt;
	bool played_bang;//po kazdem bangu se rovnou podivame, jestli mame Volcanic a pripadne zmenime tuto hodnotu
	std::string name;
	std::vector<Card> cards_hand;
	std::vector<Card> cards_desk;//modre karty, ktere jsou na stole
	std::vector<Action> actions;
	Game* g;
};

#endif