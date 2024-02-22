#include "game.h"
#include "ai.h"

void Ketchum::discard_phase()
{
    //nema smysl vyhazovat karty
    if (max_health == health || pd.cards_hand.size() < 2)
	{
		Player::discard_phase();
	}
	else
	{
        //vyhazujeme karty od nejmene podstatnych, coz rozhoduji zivoty hrace
        while (pd.cards_hand.size() > 1 && max_health > health)
		{
            for(int i = 0; i < 2; i++)
            {
                if (health > max_health / 2)
                {
                    bool result = (Ai::discard_card(pd.g, pd.cards_hand, NEU) ? true : false);
                    result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, DEF));
                    result = (result ? true : Ai::discard_blue(pd.g, pd.cards_hand));
                    result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, AGR));
                }
                else
                {
                    bool result = (Ai::discard_card(pd.g, pd.cards_hand, NEU) ? true : false);
                    result = (result ? true : Ai::discard_blue(pd.g, pd.cards_hand));
                    result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, AGR));
                    result = (result ? true : Ai::discard_card(pd.g, pd.cards_hand, DEF));
                }
            }
            health++;
		}

        if(pd.cards_hand.size() > (size_t)health)
        {
            Player::discard_phase();
        }
    }
}

void Ketchum::draw_phase()
{
    pd.discarded = 0;
    Player::draw_phase();
}

void Ketchum::ability()
{
    if(pd.discarded >= 2 && health < max_health && !pd.isai && pd.g->mode == NONE)
    {
        pd.discarded -= 2;
        health++;
    }
}
