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

bool Player::resolve_jail()//todo
{
	int a = 0;
	return true;
}

bool Player::resolve_dyn()//todo
{
	int a = 0;
	return true;
}

void Player::set_role(char r)
{
	role = r;
	if (role == 'S')
	{
		max_healt++;
		health++;
	}
}

char Player::say_role()
{
	return (role == 'S' ? 'S' : '?');
}
void Player::take_card(Card& c)
{
	cards_hand.push_back(c);
}
void Player::set_enemy(int sheriff)//todo
{
	switch (role)
	{
		case 'B':
			enemies_id.push_back(sheriff);
			break;
		case 'O':
			break;
		default:
			break;
	}
}