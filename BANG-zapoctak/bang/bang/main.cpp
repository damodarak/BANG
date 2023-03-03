#include <iostream>

#include "game/game.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Kdo zacal, pul dila vykonal." << endl; 

    Game g;
    g.load_characters();

    return 0;
}