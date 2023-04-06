#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <random>

#include "player.h"
#include "characters.h"
#include "gametools.h"

typedef std::unique_ptr<Player> Hrac;

class Game {
public:
    Game() : player_count(0), player_alive(0), notai(0), mode(""), neu_turn(-1), duel_active_turn(false),
        active_player(0) {}
    Card draw_from_deck();//liznuti jedne karty z balicku
	void draw_cards_start();
    void create(int players, std::string roles);
	void create_players(int count);
    bool finished();//urci konec hry
    void set_initial_enemies();//nastavi ze zacatku nepratele pro AI
    void set_distances();//zasadni funkce, nastavi hodnoty orientovaneho multigrafu
	void change_distance(int id1, int change, int id2 = -1);
    void game_loop();//0-hra pokracuje, 404-konec hry
    std::string id_to_name(int id);
    void vulture_sam(std::vector<Card>& reward);//schopnost jedne postavy ziskat karty po mrtvemu hraci
    void resolve_played_card();//mode != ""
    int id_to_pos(int id);
    void killed(int id);//odstani mrtveho hrace z game_order a zmeni hodnoty pro beh hry
    void resolve_notai_play();
    void ai_react();
    bool can_respond_with_card(std::string name);
    void resolve_notai_react(size_t c_index);

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
    size_t notai;
    std::vector<Hrac> game_order;//zivi hraci
    std::vector<Hrac> dead;
    std::string mode;//emporio,kulomet,indiani,bang,duel,...
    std::map<int, std::map<int, int>> distances;//orientovany multigraf
    int neu_turn;//mode == "Indiani"/"Kulomet"/"Hokynarstvi".....pokud zrovna neni zadny mode, tak neu_turn == -1
    bool duel_active_turn;//mode == "Duel"
    int active_player;//hrac, ktery je na tahu

    friend class MainWindow;
private:
    std::vector<Hrac> characters;  
};

#endif
