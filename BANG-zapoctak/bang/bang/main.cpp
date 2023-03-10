#include "game/game.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Kdo zacal, pul dila vykonal." << endl; 

    Game g;
    g.load_characters();
    g.load_cards();

    g.create(4);
    g.rotate_serif();
    g.draw_cards_start();
    g.set_initial_enemies();
    g.set_distances();

    return 0;
}