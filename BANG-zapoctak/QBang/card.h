#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <climits>
#include <QString>

class Card {
public:
	Card() : id(INT_MAX), name(""), edge('.'), suit(""), rank(INT_MAX), card_type("") {}
	Card(int id, const std::string& name, char edge, const std::string& suit, int rank, const std::string& type) : id(id), 
        name(name), edge(edge), suit(suit), rank(rank), card_type(type), dyn_active(false)
	{
        //je-li to zbran, tak ma i dosah, ostatni karty nemaji dosah
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
    QString file_loc();
    QString suit_loc();
    QString rnk();
    bool need_target();

	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
	std::string suit;
	int rank;
	std::string card_type;//Agro, Def, Neut
    bool dyn_active;//dynamit doutna, takze nemuze bouchnout

	int range;//jedine v pripade, ze se jedna o zbran
};



#endif
