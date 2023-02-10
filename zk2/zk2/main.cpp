#include "zk2.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<string> arg(argv, argv + argc);

    Graph g;
    g.handle_args(arg);
    g.process_data();
    string line = g.handle_pair();

    while (line != "KONEC")
    {
        g.print(line);
        line = g.handle_pair();
    }

    return 0;
}