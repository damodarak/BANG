#include "card.h"
#include <string>
#include <cctype>

using namespace std;

QString Card::file_loc()
{
    string cname = name;
    for(auto&& c : cname)
    {
        c = tolower(c);
    }

    string end = ":/cards/cards/" + cname + ".png";
    return QString::fromStdString(end);
}
