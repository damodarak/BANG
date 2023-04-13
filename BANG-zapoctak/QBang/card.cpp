#include "game.h"
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
    return ":/suits/suits/" + Suit_names[suit] + ".png";
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

bool Card::play(Game *g, int position, PlayerData& pd)
{
    //karty s hnedym okrajem
    switch(mode)
    {
    case BANG:
        break;
    case VEDLE:
        break;
    case PIVO:
        break;
    case SALON:
        break;
    case WELLSFARGO:
        break;
    case DOSTAVNIK:
        break;
    case HOKYNARSTVI:
        break;
    case PANIKA:
        break;
    case BALOU:
        break;
    case INDIANI:
        break;
    case KULOMET:
        break;
    case DUEL:
        break;
    }
    return false;
}

//DYNAMIT, VEZENI, BAREL, MUSTANG, APPALOOSA, VOLCANIC, SCHOFIELD, REMINGTON, CARABINE, WINCHESTER
