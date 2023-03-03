#include "game.h"

using namespace std;

void Game::load_characters()
{
	Player* pl1 = new Willy();
	characters.push_back(pl1);






	auto rng = default_random_engine{};
	shuffle(begin(characters), end(characters), rng);//pro nahodny vyber postav

}