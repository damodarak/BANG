#ifndef NOTAIHANDLE_H
#define NOTAIHANDLE_H

class Game;

class NotaiHandle {
public:
    static void resolve_notai_play(Game* g);

private:
    NotaiHandle();

    static void gun(Game* g);
    static void jail(Game* g, int enemy_pos);
    static void blue(Game* g);
};


#endif // NOTAIHANDLE_H
