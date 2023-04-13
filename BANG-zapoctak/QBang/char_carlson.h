#ifndef KIT_CARLSON_H
#define KIT_CARLSON_H

#include "player.h"

//AI-vybere si 2 karty ze 3 pomoci fce choose()
//NOTAI-3 karty se nactou do hokynarstvi a pomoci comboboxu si hrac vybere

class Carlson : public Player {
public:
    Carlson(Game* game) : Player(6, 4, game) {}
    virtual void draw_phase() override;
    virtual void ability() override;
};

#endif
