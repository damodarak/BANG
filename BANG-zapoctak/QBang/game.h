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
    Game() : player_count(0), player_alive(0), notai(0), mode(""), ai_drawed(false), active_player(0) {}
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
    int game_loop();//0-notai turn, 1-notai react, 2-notai choose emporio, 3-ai play
    int name_to_id(const QString& name);

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
    size_t notai;
    std::vector<Hrac> game_order;
    std::string mode;//emporio,duel,kulomet,indiani,none
    std::map<int, std::map<int, int>> distances;
    bool ai_drawed;

    friend class MainWindow;
private:	
    std::vector<Hrac> characters;
    int active_player;
};

#endif
