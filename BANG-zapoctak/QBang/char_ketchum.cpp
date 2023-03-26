#include "game.h"

void Ketchum::discard_phase()
{
    if (max_health == health || (cards_hand.size() - health) == 1)
	{
		Player::discard_phase();
	}
	else
	{
        while ((cards_hand.size() - health) > 1 && max_health > health)
		{
            for(int i = 0; i < 2; i++)
            {
                if (health > max_health / 2)
                {
                    bool result = (discard_card("neu") ? true : false);
                    result = (result ? true : discard_card("def"));
                    result = (result ? true : discard_blue());
                    result = (result ? true : discard_card("agr"));
                }
                else
                {
                    bool result = (discard_card("neu") ? true : false);
                    result = (result ? true : discard_blue());
                    result = (result ? true : discard_card("agr"));
                    result = (result ? true : discard_card("def"));
                }
            }
            health++;
		}

        if((cards_hand.size() - health) > 0)
        {
            Player::discard_phase();
        }
	}
}
