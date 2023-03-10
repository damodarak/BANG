#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

class Card {
public:
	Card() : id(INT_MAX), name(""), edge('.'), suit(""), rank(INT_MAX), card_type("") {}
	Card(int id, const std::string& name, char edge, const std::string& suit, int rank, const std::string& type) : id(id), 
		name(name), edge(edge), suit(suit), rank(rank), card_type(type) {};
	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
	std::string suit;
	int rank;
	std::string card_type;//Agro, Def, Neut
};



#endif