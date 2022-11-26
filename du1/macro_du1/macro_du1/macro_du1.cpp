#include <cctype>
#include "macro_du1.h"

using namespace std;

void Macro::handlecl(const vector<string>& arg)
{
    string nazev = arg[1];
    string telo = arg[2];
    int size = arg.size();
    for (int i = 3; i < size; i++)
    {
        telo += " " + arg[i];
    }
    macra_dict.insert({ nazev,telo });
}

string Macro::next(istream& s)
{
    char c;
    word = "";

    for (;;) {
        if (ready)
        {
            ready = false;
            return word;
        }

        c = s.get();

        if (s.fail())
        {
            return word;
        }
        else
        {
            process(c);
        }
    }
}

bool Macro::control(char& c)
{
    if ((next_must == "alpha" && !isalpha(c)) || (next_must == "space" && !isspace(c)))
    {
        ready = true;
        error = true;
        return false;
    }

    next_must = "";
    return true;
}

void Macro::process(char& c)
{
    if (!control(c))//kontrola spravnosti
    {
        return;
    }
    else if (inmacro)
    {
        if (c == '#')
        {
            if (macro_last_space)
            {
                inmacro = false;
                process_macro();
            }
            else
            {
                error = false;
                ready = true;
            }
        }
        else
        {
            macro_last_space = isspace(c);
            macro += c;
        }
    }
    else//nejsme v macru
    {
        process_notmacro(c);
    }
}

void Macro::process_notmacro(char& c)
{
    if (c == '#' && last_space)
    {
        next_must = "alpha";
        inmacro = true;
        ready = true;
    }
    else if (inword)
    {
        if (isspace(c))
        {
            //check jestli macro
            last_space = true;
            inword = false;
            if (macra_dict.find(word) == macra_dict.end())
            {
                word += c;
                ready = true;
            }
            else
            {
                word = macra_dict.find(word)->second + c;
                ready = true;
            }
        }
        else if (isalnum(c))
        {
            word += c;
        }
        else//skoncilo slovo...kontrola, jestli existuje makro
        {
            ready = true;
            if (macra_dict.find(word) == macra_dict.end())
            {
                word += c;
            }
            else
            {
                word = macra_dict.find(word)->second + c;
            }
        }
    }
    else//nejsme uvnitr slova
    {
        word += c;
        if (last_space)
        {
            last_space = isspace(c);
            if (isspace(c))
            {
                ready = true;
            }
            else if (isalpha(c))
            {
                inword = true;
            }
        }
        else//posledni znak nebyl isspace a nejsme ve slove
        {
            last_space = isspace(c);
            ready = isspace(c);
        }
    }
}

void Macro::process_macro()
{
    int telo_start = macro.find(" ");
    string nazev = macro.substr(0, telo_start);
    int index = telo_start + 1;
    string definice = "";

    bool inword = false;
    string temp = "";
    int size = macro.size();

    for (;index < size; index++)
    {
        if (inword)
        {
            if (isalnum(macro[index]))
            {
                temp += macro[index];
            }
            else if (macra_dict.find(temp) == macra_dict.end())
            {
                definice += temp + macro[index];
                inword = false;
                temp = "";
            }
            else
            {
                definice += macra_dict.find(temp)->second + macro[index];
                inword = false;
                temp = "";
            }
        }
        else
        {
            if (isalnum(macro[index]))
            {
                temp += macro[index];
                inword = true;
            }
            else
            {
                temp += macro[index];
                definice += temp;
                temp = "";
            }
        }
    }

    definice += temp;
    macra_dict.insert({ nazev, definice.substr(0, definice.size() - 1) });//nechceme posledni mezeru
    macro = "";
    next_must = "space";
}