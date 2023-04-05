#include "game.h"
#include "ai.h"

void Ketchum::discard_phase()
{
    //nema smysl vyhazovat karty
    if (max_health == health || cards_hand.size() < 2)
	{
		Player::discard_phase();
	}
	else
	{
        //vyhazujeme karty od nejmene podstatnych, coz rozhoduji zivoty hrace
        while (cards_hand.size() > 1 && max_health > health)
		{
            for(int i = 0; i < 2; i++)
            {
                if (health > max_health / 2)
                {
                    bool result = (Ai::discard_card(g, cards_hand, "neu") ? true : false);
                    result = (result ? true : Ai::discard_card(g, cards_hand, "def"));
                    result = (result ? true : Ai::discard_blue(g, cards_hand));
                    result = (result ? true : Ai::discard_card(g, cards_hand, "agr"));
                }
                else
                {
                    bool result = (Ai::discard_card(g, cards_hand, "neu") ? true : false);
                    result = (result ? true : Ai::discard_blue(g, cards_hand));
                    result = (result ? true : Ai::discard_card(g, cards_hand, "agr"));
                    result = (result ? true : Ai::discard_card(g, cards_hand, "def"));
                }
            }
            health++;
		}

        if(cards_hand.size() > (size_t)health)
        {
            Player::discard_phase();
        }
    }
}

void Ketchum::draw_phase()
{
    discarded = 0;
    Player::draw_phase();
}

void Ketchum::ability()
{
    if(discarded >= 2 && health < max_health && !g->game_order[g->active_player]->isai && g->mode == "")
    {
        discarded -= 2;
        health++;
    }
}
