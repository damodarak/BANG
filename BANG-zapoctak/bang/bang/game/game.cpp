#include <fstream>

#include "game.h"

using namespace std;

void Game::load_cards()
{
	std::ifstream f;
	f.open("postavy.txt");
	string line;
	getline(f, line);
	for (size_t i = 0; i < 16; i++)
	{
		getline(f, line);
		size_t found1 = line.find(",");
		size_t found2 = line.find(",", found1);

		if (line[0] == 'W')
		{
			Willy player(4);
		}
	}
	f.close();
}