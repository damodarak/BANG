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
            }
            else//nejsme uvnitr slova
            {
                if (last_space)
                {
                    if (isspace(c))
                    {
                        word += c;
                        ready = true;
                    }
                    else if(isalpha(c))
                    {
                        inword = true;
                        last_space = false;
                        word += c;
                    }
                    else
                    {
                        last_space = false;
                        word += c;
                    }
                }
                else//posledni znak nebyl isspace a nejsme ve slove
                {
                    if (isspace(c))
                    {
                        word += c;
                        last_space = true;
                        ready = true;
                    }
                    else
                    {
                        last_space = false;
                        word += c;
                    }
                }
            }
        }
    }
    void process_macro()
    {
        //dodelat vnorene macro
        //test if macro fulfils conditions mezery pred a po #
        int body = macro.find(" ");
        string telo = macro.substr(0, body);
        macra_dict.insert({ telo, macro.substr(body + 1, macro.size() - 2 - telo.size()) });
        macro = "";
        next_must = "space";
    }
};

int main(int argc, char** argv)
{
    vector<string> arg(argv, argv + argc);
    Macro m;
    //# na vstupu za jiným znakem než isspace se chová jako bìžný znak

    string str;

    for (;;) {
        if (m.fail(cin))
        {
            cout << "------------------\nKONEEEEEEEEEEEEEEEEEEEEEEC!!!!!!" << endl;
            return 0;
        }
        else
        {
            str = m.next(cin);
            cout << str;
        }
    }

}