#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <set>
#include <QLabel>
#include <QList>
#include <QString>

#include "card.h"

enum Action { DRAW, PlAY, DISCARD, DISCARD_RANDOM_CARD, PLAY_MISS, PLAY_2MISS, PLAY_BANG, DRAW_EMPORIO  };

class Game;

static int next_player_id = 0;

class Player {
public:
    Player(int rank, int max_hp, const std::string& char_name, Game* game) : isai(true),
        ranking(rank), id(++next_player_id), health(max_hp), max_health(max_hp), played_bang(false),
        drawed(false), ability_used(false), played_vedle(0), name(char_name), role('?'), g(game) {}
	virtual void draw_phase();
    virtual int game_phase()
    {
        return 0;
    }
    virtual void ability()
    {

    }
	virtual void discard_phase();
    virtual bool dec_hp(int lifes) {health -= lifes; return health > 0;}
	char say_role();
	void set_role(char r);
	void take_card(Card& c);
	void set_enemy(int sheriff, const std::vector<int>& ids);
    virtual Card give_random_card();
    virtual Card give_random_card_hand();
	virtual bool resolve_jail();
	virtual bool resolve_dyn();
	virtual bool resolve_barrel();
    int card_count();
    QString file_loc();
    QString role_loc();
    int has_gun();

	bool isai;
	int ranking;//for AI to choose beter character

	int id;
	int health;
    int max_health;
	bool played_bang;//po kazdem bangu se rovnou podivame, jestli mame Volcanic a pripadne zmenime tuto hodnotu
    bool drawed;
    bool ability_used;
    int played_vedle;
    std::string name;
	std::vector<Card> cards_desk;//modre karty, ktere jsou na stole

    friend class MainWindow;
    friend class Game;
protected:
	char role;
	std::vector<Card> cards_hand;
	std::vector<Action> actions;
	std::set<int> enemies_id;
	Game* g;

    QList<QLabel *> m_l;
    QList<QLabel *> cards_l;
    QLabel* card_l;
    QLabel* char_l;
    QLabel* role_l;
    QLabel* hp_l;
    QLabel* count_l;

	bool discard_blue();
	bool discard_card(const std::string& type);
    void discard_card(int index);
    int index(const std::vector<Card>& cards, const std::string& name);
    int choose(const std::vector<Card>& cards);
};

#endif
