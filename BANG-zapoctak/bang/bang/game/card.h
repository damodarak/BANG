#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

class Card {
public:
	Card() {}
	Card(int id, std::string name, char edge, std::string suit, int rank) : id(id), name(name), edge(edge), suit(suit), rank(rank) {};
	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
	std::string suit;
	int rank;
	std::vector<char> card_type;//Agro, Def, Neut
};



#endif