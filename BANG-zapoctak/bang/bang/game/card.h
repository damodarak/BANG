#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

class Game;

class Card {
public:
	virtual void Play(Game* g) = 0;
protected:
	int id;
	std::string name;
	std::string suit;
	int rank;
	std::vector<char> card_type;//Agro, Def, Neut
};



#endif