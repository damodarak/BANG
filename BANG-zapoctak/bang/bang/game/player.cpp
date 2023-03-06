#include "player.h"
#include "game.h"

void Player::draw_phase()
{
	Card c;
	for (size_t i = 0; i < 2; i++)
	{
		c = g->draw_from_deck();
		cards_hand.push_back(c);
	}
}

bool Player::resolve_jail()
{
	int a = 0;
	return true;
}

bool Player::resolve_dyn()
{
	int a = 0;
	return true;
}

void Player::set_role(char r)
{
	role = r;
}

char Player::say_role()
{
	return (role == 'S' ? 'S' : '?');
}