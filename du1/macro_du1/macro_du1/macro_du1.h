#include <map>
#include <string>
#include <vector>
#include <iostream>

#ifndef MACRO_DU1_H_
#define MACRO_DU1_H_

class Macro 
{
public:
    Macro()
    {
        inword = inmacro = ready = error = macro_last_space = false;
        last_space = true;
        word = "";
        macro = "";
        next_must = "";
    }
    void handlecl(const std::vector<std::string>& arg);
    bool fail() {return error;}
    std::string next(std::istream& s);
private:
    bool control(char& c);
    void process(char& c);
    void process_notmacro(char& c);
    void process_macro();

    bool inword;
    bool inmacro;
    bool last_space;
    bool ready;
    bool error;
    bool macro_last_space;

    std::string word;
    std::string macro;
    std::string next_must;

    std::map<std::string, std::string> macra_dict;
};

#endif