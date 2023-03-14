#include "../game.h"

void Ketchum::discard_phase()//todo
{
	if (max_healt == health || (cards_hand.size() - health) == 1)
	{
		Player::discard_phase();
	}
	else
	{
		while ((cards_hand.size() - health) % 2 == 0)
		{

		}
	}
}