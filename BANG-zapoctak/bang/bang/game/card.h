#ifndef CARD_H
#define CARD_H

#include <string>

class Game;

class Card {
public:
	virtual void Play(Game* g) = 0;
protected:
	int id;
	std::string name;
	std::string suit;
	int rank;
};



#endif