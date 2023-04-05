#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <set>

#include "card.h"

class Game;

static int next_player_id = 0;

class Player {
public:
    Player(int rank, int max_hp, const std::string& char_name, Game* game) : layout_index(0), isai(true),
        ranking(rank), id(++next_player_id), health(max_hp), max_health(max_hp), discarded(0), played_bang(false),
        drawed(false), ability_used(false), barel(0), played_vedle(0), name(char_name), role('?'), g(game), target_id(-1) {}
    virtual void draw_phase();
    virtual int game_phase();//0-koncim, 1-chci pokracovat po vyreseni odehrane karty, 2-chci hrat hned
    virtual void ability()//jenom 5 postav ma schopnost, kterou muze notAI vyvolat tlacitkem
    {

    }
    virtual void discard_phase();//odhozeni karet, aby platilo cards <= health
    virtual bool dec_hp(int lifes);//decresase health
	char say_role();
	void set_enemy(int sheriff, const std::vector<int>& ids);
    virtual Card give_random_card();
    virtual Card give_random_card_hand();
	virtual bool resolve_jail();
	virtual bool resolve_dyn();
	virtual bool resolve_barrel();
    virtual bool play_bang();//vynuceni odehrani bangu nebo ekvivalentu
    virtual bool play_vedle();
    virtual bool resolve_slab_bang();//schopnost Slaba the Killera
    int card_count();
    std::string file_loc();
    std::string role_loc();
    int has_gun();//vrati -1 pokud hrac nema zbran nebo range zbrane
    void dostavnik_wells(int count);
    int hand_size();
    bool panika_balou_play(int enemy_id);
    void add_enemy_vice(int enemy_id);//pokud nekdo zautoci na serifa tak i jeho pomocnici to zaznamenaji
    bool has_notai_ability();
    bool has_dyn();
    bool has_jail();

    int layout_index;//index of QVector<Qlist<QLabel*>> with labels for blue cards, role,
	bool isai;
	int ranking;//for AI to choose beter character

	int id;
	int health;
    int max_health;
    int discarded;
	bool played_bang;//po kazdem bangu se rovnou podivame, jestli mame Volcanic a pripadne zmenime tuto hodnotu
    bool drawed;
    bool ability_used;
    int barel;
    int played_vedle;
    std::string name;
	std::vector<Card> cards_desk;//modre karty, ktere jsou na stole

    friend class MainWindow;
    friend class Game;
    friend class Cringo;
protected:
	char role;
	std::vector<Card> cards_hand;
	std::set<int> enemies_id;
	Game* g;
    int target_id;

    bool discard_blue();//vyhazovani karet
	bool discard_card(const std::string& type);
    void discard_card(size_t index);
    int index(const std::vector<Card>& cards, const std::string& name);//nalezeni karty ve vectoru karet
    int choose(const std::vector<Card>& cards);
    void set_target_id(const std::string& name);
    bool has_blue(const std::string& name);
    int exist_enemy_jail();
    void pass_jail(int c_index, int enemy_id);
    int best_gun_hand();
    bool play_neu(const std::string& name);//rozhodne zda-li je vhodne hrat neutralni kartu
    bool can_play_panika(int enemy_id);
    std::vector<Card> give_all_cards();
    void turn_reset();//na konci kola kazdeho hrace
};

#endif
