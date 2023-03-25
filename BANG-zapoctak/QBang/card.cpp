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

QString Card::suit_loc()
{
    string end = ":/suits/suits/" + suit + ".png";
    return QString::fromStdString(end);
}

QString Card::rnk()
{
    if(rank < 11)
    {
        return QString::number(rank);
    }
    else
    {
        QString end;
        switch(rank)
        {
        case 11:
            end += 'J';
            break;
        case 12:
            end += 'Q';
            break;
        case 13:
            end += 'K';
            break;
        case 14:
            end += 'A';
            break;
        }
        return end;
    }
}
