#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <random>

#include "player.h"
#include "characters.h"
#include "gametools.h"
#include "notaihandle.h"

typedef std::unique_ptr<Player> Hrac;

class Game {
public:
    Game() : player_count(0), player_alive(0), notai(0), mode(NONE), neu_turn(-1), duel_active_turn(false),
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
    void vulture_sam(std::vector<Card>& reward);//schopnost jedne postavy ziskat karty po mrtvemu hraci
    void killed(int id);//odstani mrtveho hrace z game_order a zmeni hodnoty pro beh hry
    bool can_respond_with_card(int index);
    void next_neu();
    void pre_draw_phase();

	std::deque<Card> deck;//front->draw, back->discard
	std::vector<Card> emporio;//odsud se budou brat karty po pouziti karty Hokynarstvi
	int player_count;
	int player_alive;
    size_t notai;
    std::vector<Hrac> game_order;//zivi hraci
    std::vector<Hrac> dead;
    Modes mode;//emporio,kulomet,indiani,bang,duel,...
    std::map<int, std::map<int, int>> distances;//orientovany multigraf
    int neu_turn;//mode == "Indiani"/"Kulomet"/"Hokynarstvi".....pokud zrovna neni zadny mode, tak neu_turn == -1
    bool duel_active_turn;//mode == "Duel"
    int active_player;//hrac, ktery je na tahu
private:
    std::vector<Hrac> characters;
};

#endif
