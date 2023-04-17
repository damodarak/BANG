#include "game.h"
#include "ai.h"

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

bool Card::play(int position, PlayerData& pd)
{
    //karty s hnedym okrajem
    switch(mode)
    {
    case BANG:
        return Ai::bang(position, pd);
    case VEDLE:
        return pd.ranking == CALAMITY && Ai::bang(position, pd);
    case PIVO:
        return Ai::beer(position, pd.g);
    case WELLSFARGO:
        GameTools::dostavnik_wells(pd.g, pd.g->active_player, 3);
        return true;
    case DOSTAVNIK:
        GameTools::dostavnik_wells(pd.g, pd.g->active_player, 2);
        return true;
    case SALON:
        return Ai::neu(pd.g, SALON);
    case HOKYNARSTVI:
        return Ai::neu(pd.g, HOKYNARSTVI);
    case INDIANI:
        return Ai::neu(pd.g, INDIANI);
    case KULOMET:
        return Ai::neu(pd.g, KULOMET);
    case PANIKA:
        return Ai::panika(pd, pd.g->game_order[position]->target_id, pd.g->game_order[position]->id);
    case BALOU:
        return Ai::balou(pd, pd.g->game_order[position]->target_id);
    case DUEL:
        return Ai::duel(pd, pd.g->game_order[position]->target_id);
    default:
        return false;
    }
    //DYNAMIT, VEZENI, BAREL, MUSTANG, APPALOOSA, VOLCANIC, SCHOFIELD, REMINGTON, CARABINE, WINCHESTER
}
