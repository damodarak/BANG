#ifndef CARD_HORSE_H
#define CARD_HORSE_H

#include "card.h"

class Horse : public Card {
public:
	Horse(int id, std::string name, char edge, char suit, int rank) : Card(id, name, edge, suit, rank) {};
	void Play(int id, Game* g);//set distances
};

#endif