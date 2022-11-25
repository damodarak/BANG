#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

class Macro {
public:
    Macro()
    {
        inword = inmacro = ready = error = macro_last_space = false;
        last_space = true;
        word = "";
        macro = "";
        next_must = "";
    }
    void handlecl(const vector<string>& arg)
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
    bool fail(istream& s)
    {
        return error || s.fail();
    }
    string next(istream& s)
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

private:
    bool inword;
    bool inmacro;
    bool last_space;
    bool ready;
    bool error;
    bool macro_last_space;

    string word;
    string macro;
    string next_must;

    map<string, string> macra_dict;

    bool control(char& c)
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

    void process(char& c)
    {
        if (!control(c))
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
                    if (isspace(c))
                    {
                        ready = true;
                    }
                    else if(isalpha(c))
                    {
                        inword = true;
                        last_space = false;
                    }
                    else
                    {
                        last_space = false;
                    }
                }
                else//posledni znak nebyl isspace a nejsme ve slove
                {
                    last_space = isspace(c);
                    ready = isspace(c);
                }
            }
        }
    }
    void process_macro()
    {
        int telo_start = macro.find(" ");
        string nazev = macro.substr(0, telo_start);
        int index = telo_start + 1;
        string definice = "";

        bool inword = false;
        string temp = "";
        int size = macro.size();

        for (;index<size; index++)
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


                    inword = false;
                    temp = "";
                }

            }
            else
            {

            }
        }



        macra_dict.insert({nazev, definice});
        macro = "";
        next_must = "space";
    }
};

int main(int argc, char** argv)
{
    Macro m;
    vector<string> arg(argv, argv + argc);
    if (arg.size()>1)
    {
        m.handlecl(arg);
    }
    
    string str;

    for (;;) {
        if (m.fail(cin))
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