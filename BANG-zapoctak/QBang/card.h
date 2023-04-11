#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <climits>

enum Modes {NONE, BANG, VEDLE, EMPORIO, PANIKA, BALOU, INDIANI, SALON, KULOMET, DUEL, SLAB, CARLSON};

class Card {
public:
	Card() : id(INT_MAX), name(""), edge('.'), suit(""), rank(INT_MAX), card_type("") {}

    Card(int id, const std::string& name, char edge, const std::string& suit,
         int rank, const std::string& type, int range, int mode) : id(id),
        name(name), edge(edge), suit(suit), rank(rank), card_type(type),
        dyn_active(false), range(range), mode(mode) {}

    std::string file_loc();
    std::string suit_loc();
    std::string rnk();
    bool need_target();

	int id;
	std::string name;
	char edge;//Modry, Hnedy okraj
	std::string suit;
	int rank;
	std::string card_type;//Agro, Def, Neut
    bool dyn_active;//dynamit doutna, takze nemuze bouchnout
	int range;//jedine v pripade, ze se jedna o zbran
    int mode;
};

#endif
