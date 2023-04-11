#ifndef AI_H
#define AI_H

#include <vector>
#include <string>

class Game;
class Card;

class Ai {
public:
    static bool discard_blue(Game* g, std::vector<Card>& cards);
    static bool discard_card(Game* g, std::vector<Card>& cards, const std::string& type);
    static void discard_card(Game* g, std::vector<Card>& c_hand, std::vector<Card>& c_desk, size_t index);
    static int index(const std::vector<Card>& cards, const std::string& name_type);//nalezeni karty ve vectoru karet
    static bool can_play_panika(Game* g, int id, int enemy_id);
    static bool play_neu(Game* g, const std::string& name);//zda-li ai ma hrat neu kartu
    static int best_gun(std::vector<Card>& cards);//vrati -1 pokud hrac nema zbran nebo index v cards
    static int has_gun(std::vector<Card>& cards);//vrati -1 pokud hrac nema zbran nebo range zbrane
    static void vice_add_enemy(Game* g, int enemy_id);//pokud nekdo zautoci na serifa tak i jeho pomocnici to zaznamenaji
    static bool panika_balou_play(Game* g, int enemy_id);
    static void cringo_abil(Game* g);
    static bool jesse_abil(Game* g);

private:
    //staticka trida
    Ai() {}
};

#endif // AI_H
