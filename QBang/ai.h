#ifndef AI_H
#define AI_H

#include <vector>
#include <set>

class Game;
class Card;
class PlayerData;

class Ai {
public:
    static bool discard_blue(Game* g, std::vector<Card>& cards);
    static bool discard_card(Game* g, std::vector<Card>& cards, int type);
    static void discard_card(Game* g, std::vector<Card>& c_hand, std::vector<Card>& c_desk, size_t index);
    static int index_name(const std::vector<Card>& cards, int name);//nalezeni karty ve vectoru karet
    static int index_type(const std::vector<Card>& cards, int type);
    static bool can_play_panika(Game* g, int id, int enemy_id);
    static bool play_neu(Game* g, int name);//zda-li ai ma hrat neu kartu
    static int best_gun(std::vector<Card>& cards);//vrati -1 pokud hrac nema zbran nebo index v cards
    static int has_gun(std::vector<Card>& cards);//vrati -1 pokud hrac nema zbran nebo range zbrane
    static void vice_add_enemy(Game* g, int enemy_id);//pokud nekdo zautoci na serifa tak i jeho pomocnici to zaznamenaji
    static bool panika_balou_play(Game* g, int enemy_id);
    static void cringo_abil(Game* g);
    static bool jesse_abil(Game* g);
    static bool no_pedro_abil(Game* g, int id);
    static bool no_jourd_abil(Game* g, int barel);
    static bool bang(int position, PlayerData& pd);
    static bool beer(int position, Game* g);
    static bool neu(Game* g, int mode);
    static bool duel(PlayerData& pd, int& target_id);
    static bool balou(PlayerData& pd, int& target_id);
    static bool panika(PlayerData& pd, int& target_id, int id);
    static void enemy_check(PlayerData& pd);

private:
    //staticka trida
    Ai() {}
};

#endif // AI_H
