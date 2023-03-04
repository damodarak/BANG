#ifndef CARD_GUN_H
#define CARD_GUN_H

#include "card.h"

class Gun : public Card {
public:
	Gun(int id, std::string name, char edge, char suit, int rank) : Card(id, name, edge, suit, rank) {};
	void Play(int id, Game* g);//set distances
};

#endif