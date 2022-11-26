#include "macro_du1.h"

using namespace std;

int main(int argc, char** argv)
{
    Macro m;
    vector<string> arg(argv, argv + argc);
    if (arg.size() > 1)
    {
        m.handlecl(arg);
    }

    string str;

    for (;;) {
        if (cin.fail())
        {
            return 0;
        }
        if (m.fail())
        {
            cout << "Error" << endl;
            return 0;
        }
        else
        {
            str = m.next(cin);
            cout << str;
        }
    }
}