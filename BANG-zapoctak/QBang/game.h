#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <QVector>

#include "player.h"
#include "characters.h"

typedef std::unique_ptr<Player> Hrac;

class Game {
public:
    Game() : player_count(0), player_alive(0), active_player_id(0) {};
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
    void clear();
    void add_labels(QVector<QList<QLabel*>>& layout);

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
    std::vector<Hrac> characters;
private:
	std::vector<Hrac> game_order;
	std::map<int, std::map<int, int>> distances;
	int active_player_id;
};

#endif