#include "card.h"
#include <string>
#include <cctype>

using namespace std;

string Card::file_loc()
{
    string cname = name;
    for(auto&& c : cname)
    {
        c = tolower(c);
    }

    return cname + ".png";
}
