#include "game.h"
#include "ai.h"

void Pedro::draw_phase()
{
    //pokud zbyva posledni hrac mimo nas tak ho pridame na seznam nepratel
    if(pd.g->game_order.size() == 2)
    {
        pd.enemies_id.insert(pd.g->game_order[(pd.g->active_player + 1) % 2]->id);
    }
    //Kdyby nahodou si tam pomocnik serifa dal serifa
    if(pd.role == 'V' && pd.enemies_id.find(pd.g->game_order[0]->id) != pd.enemies_id.end())
    {
        pd.enemies_id.erase(pd.enemies_id.find(pd.g->game_order[0]->id));
    }

    pd.ability_used = true;
    pd.drawed = true;

    //zjistujeme jestli nas zajima dana karta nebo jestli nam nepomuze ze situace
    if(pd.g->deck.back().mode == DOSTAVNIK || pd.g->deck.back().mode == WELLSFARGO)
    {
        pd.cards_hand.push_back(pd.g->draw_from_deck());
        pd.cards_hand.push_back(pd.g->deck.back());
        pd.g->deck.pop_back();
        return;
    }
    else if((health > max_health / 2 && pd.g->deck.back().card_type == AGR) ||
            (health <= max_health / 2 && pd.g->deck.back().card_type == DEF))
    {
        pd.cards_hand.push_back(pd.g->draw_from_deck());
        pd.cards_hand.push_back(pd.g->deck.back());
        pd.g->deck.pop_back();
        return;
    }
    else
    {
        Player::draw_phase();
    }
}

void Pedro::ability()
{
    if(Ai::no_pedro_abil(pd.g, id))
    {
        return;
    }
    pd.cards_hand.push_back(pd.g->draw_from_deck());
    pd.cards_hand.push_back(pd.g->deck.back());
    pd.g->deck.pop_back();
    pd.ability_used = true;
    pd.drawed = true;
}
