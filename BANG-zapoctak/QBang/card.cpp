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

    return ":/cards/cards/" + cname + ".png";
}

string Card::suit_loc()
{
    return ":/suits/suits/" + suit + ".png";
}

//prevod cisla na hodnost karty
string Card::rnk()
{
    if(rank < 11)
    {
        return to_string(rank);
    }
    else
    {
        string end;
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

bool Card::need_target()
{
    if((Modes)mode == VEZENI)
    {
        return true;
    }
    else if(edge == 'M')
    {
        return false;
    }
    else if((Types)card_type == AGR)
    {
        return true;
    }
    else
    {
        return false;
    }
}
