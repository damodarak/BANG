#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
	void Play();// = 0, virtual
protected:
	int id;
	std::string name;
	std::string suit;
	int rank;
};



#endif