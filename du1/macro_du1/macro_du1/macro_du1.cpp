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
        inword = inmacro = ready = error = false;
        last_space = true;
        word = "";
        macro = "";
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

    string word;
    string macro;

    map<string, string> macra_dict;

    void process(char& c)
    {
        if (inmacro)
        {
            if (c == '#')
            {
                inmacro = false;
                process_macro();
            }
            else
            {
                macro += c;
            }
        }
        else//nejsme v macru
        {
            if (c == '#' && last_space)
            {
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
                else
                {
                    word += c;
                    ready = true;
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
                else//posledni znak nebyl isspace
                {

                }
            }
        }
    }
    void process_macro()
    {
        //dodelat vnorene macro
        //test if macro fulfils conditions mezery pred a po #
        int body = macro.find(" ");
        macra_dict.insert({ macro.substr(1, body), macro.substr(body + 1, macro.size() - 1) });
        macro = "";
    }
};

int main(int argc, char** argv)
{
    vector<string> arg(argv, argv + argc);
    Macro m;
    //# na vstupu za jiným znakem než isspace se chová jako bìžný znak

    string str;

    for (;;) {
        if (cin.fail())
        {
            return 0;
        }
        else
        {
            str = m.next(cin);
            cout << str;
        }
    }

}