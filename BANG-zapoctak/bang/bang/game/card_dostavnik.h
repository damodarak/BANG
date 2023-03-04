#ifndef DOSTAVNIK_H
#define DOSTAVNIK_H

#include "game.h"

//tento objekt bude reprezentovat karty Dostavnik, Wells Fargo

class Dostavnik : public Card {
public:
	Dostavnik(int id, std::string name, char edge, std::string suit, int rank, int count) : Card(id, name, edge, suit, rank), cards_to_draw(count) {};
	std::vector<Card> Play(Game* g);//push_back 'cards_to_draw' cards to player with id
private:
	int cards_to_draw;
};

#endif