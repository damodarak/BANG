#ifndef GAMETOOLS_H
#define GAMETOOLS_H

#include <vector>
#include <memory>

class Player;
class Game;

typedef std::unique_ptr<Player> Hrac;

class GameTools{

public:
    static void load_chars(Game* g, std::vector<Hrac>& characters);
    static void load_card(Game* g, std::vector<std::string>& v);
    static void rm_enemy(Game* g, int id);//po smrti hrace odstranime jeho id ze vsech seznamu nepratel
    static void rotate_serif(Game* g);//serif je vzdy na pozici game_order[0]
    static void saloon(Game* g);//vsem prida zivot, je-li to mozne
    static void set_notai(Game* g);
    static void load_emporio(Game* g);
    static int id_to_pos(Game* g, int id);
    static void suzy_abil(Game* g);
    static void jourd_abil(Game* g);
    static size_t hand_size(Game* g, int id);
    static std::string id_to_name(Game* g, int id);
    static void dostavnik_wells(Game* g, int position, int count);
    static void discard_killed(Game* g, char role, int pos);
    static void resolve_played_card(Game* g);
private:
    //staticka trida
    GameTools() {}

    static void panika(Game* g);
    static void balou(Game* g);
    static void indiani_kulomet(Game* g);
    static void hokynarstvi(Game* g);
    static void bang(Game* g);
    static void duel(Game* g);
};


#endif // GAMETOOLS_H
