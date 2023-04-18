#ifndef NOTAIHANDLE_H
#define NOTAIHANDLE_H

#include <string>

class Game;

class NotaiHandle {
public:
    static void resolve_notai_play(Game* g);
    static void resolve_notai_react(Game* g, size_t c_index);
private:
    NotaiHandle();

    static void gun(Game* g);
    static void jail(Game* g, int enemy_pos);
    static void blue(Game* g);

    static void notai_react_vedle(Game* g, int card_mode);
    static void notai_react_slab(Game* g, int card_mode);
};

#endif // NOTAIHANDLE_H
