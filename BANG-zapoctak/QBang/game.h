#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <QVector>
#include <random>

#include "player.h"
#include "characters.h"

typedef std::unique_ptr<Player> Hrac;

class Game {
public:
    Game() : player_count(0), player_alive(0), notai(0), mode(""), neu_turn(-1), duel_active_turn(false),
        active_player(0) {}
	void load_characters();
    void load_card(std::vector<std::string>& v);
	Card draw_from_deck();
	void draw_cards_start();
    void create(int players, std::string roles);
	void create_players(int count);
	void rotate_serif();
	bool finished();
	void set_initial_enemies();
	void set_distances();
	void change_distance(int id1, int change, int id2 = -1);
    void add_labels(QVector<QList<QLabel*>>& layout);
    void add_label_ai(int ai, QList<QLabel*>& labels);
    void saloon();
    int game_loop();//0-notai turn, 1-notai react,  3-ai play, 404-konec hry
    QString id_to_name(int id);
    void vulture_sam(std::vector<Card>& reward);
    void resolve_played_card();
    int id_to_pos(int id);
    void load_emporio();
    void killed(int id);
    void set_notai();
    void resolve_notai_play();
    void ai_react();
    bool notai_duel_react();
    bool can_respond_with_card(std::string name);
    void resolve_notai_react(size_t c_index);

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
    size_t notai;
    std::vector<Hrac> game_order;
    std::vector<Hrac> dead;
    std::string mode;//emporio,kulomet,indiani,bang,duel
    std::map<int, std::map<int, int>> distances;
    int neu_turn;
    bool duel_active_turn;
    int active_player;

    friend class MainWindow;
private:	
    std::vector<Hrac> characters;  

    void rm_enemy(int id);
};

#endif
