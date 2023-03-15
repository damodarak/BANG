#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <set>

#include "card.h"

enum Action { DRAW, PlAY, DISCARD, DISCARD_RANDOM_CARD, PLAY_MISS, PLAY_2MISS, PLAY_BANG, DRAW_EMPORIO  };

class Game;

static int next_player_id = 0;

class Player {
public:
	Player(int rank, int max_hp, const std::string& char_name, Game* game) : isai(true), ranking(rank), id(++next_player_id), 
		health(max_hp), max_healt(max_hp), played_bang(false), name(char_name), role('?'), g(game) {};
	virtual void draw_phase();
    virtual void game_phase()
    {

    }
	virtual void discard_phase();
	char say_role();
	void set_role(char r);
	void take_card(Card& c);
	void set_enemy(int sheriff, const std::vector<int>& ids);
	virtual bool resolve_jail();
	virtual bool resolve_dyn();
	virtual bool resolve_barrel();

	bool isai;
	int ranking;//for AI to choose beter character

	int id;
	int health;
	int max_healt;
	bool played_bang;//po kazdem bangu se rovnou podivame, jestli mame Volcanic a pripadne zmenime tuto hodnotu
	std::string name;
	std::vector<Card> cards_desk;//modre karty, ktere jsou na stole
protected:
	char role;
	std::vector<Card> cards_hand;
	std::vector<Action> actions;
	std::set<int> enemies_id;
	Game* g;

	bool discard_blue();
	bool discard_card(const std::string& type);
};

#endif
