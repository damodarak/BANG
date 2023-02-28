#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
	Player(int id) : id_(id) {};
	virtual void draw_phase() = 0;
	virtual void game_phase() = 0;
	virtual void discard_phase() = 0;
	void choose_character();
protected:
	int id_;
};

#endif