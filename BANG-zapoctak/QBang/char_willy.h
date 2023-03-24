#ifndef WILLY_THE_KID_H
#define WILLY_THE_KID_H

#include "player.h"

class Willy : public Player {
public:
    Willy(Game* game) : Player(1, 4, "willy", game) {}
    virtual int game_phase() override
	{
        int result = Player::game_phase();
        ability();
        return result;
	}
private:
    void ability() {played_bang = false;}
};


#endif
