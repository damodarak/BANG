#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Macro {
public:
    Macro()
    {
        inword = inmacro = false;
        last_space = true;
        word = "";
        macro = "";
    }
    string next(istream& s)
    {
        char c;
        word = "";

        for (;;) {
            if (word != "" and !inword and !inmacro)
            {
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

//private:
    bool inword;
    bool inmacro;
    bool last_space;

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
        else
        {

        }
    }
    void process_macro()
    {
        int body = macro.find(" ");
        macra_dict.insert({ macro.substr(0, body), macro.substr(body + 1, macro.size()-1) });
        macro = "";
    }
};

int main(int argc, char** argv)
{
    vector<string> arg(argv, argv + argc);
    Macro m;
   
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