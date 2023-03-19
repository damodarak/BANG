#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <climits>

class Card {
public:
	Card() : id(INT_MAX), name(""), edge('.'), suit(""), rank(INT_MAX), card_type("") {}
	Card(int id, const std::string& name, char edge, const std::string& suit, int rank, const std::string& type) : id(id), 
		name(name), edge(edge), suit(suit), rank(rank), card_type(type) 
	{
		if (name == "Volcanic")
		{
			range = 1;
		}
		else if (name == "Schofield")
		{
			range = 2;
		}
		else if (name == "Remington")
		{
			range = 3;
		}
		else if (name == "Carabine")
		{
			range = 4;
		}
		else if (name == "Winchester")
		{
			range = 5;
		}
		else
		{
			range = 0;
		}
	};
	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
	std::string suit;
	int rank;
	std::string card_type;//Agro, Def, Neut

	int range;//jedine v pripade, ze se jedna o zbran
};



#endif
