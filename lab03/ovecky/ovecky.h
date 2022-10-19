#include <iostream>

#ifndef OVECKY_H_
#define OVECKY_H_

class Ovecky {
public:
    Ovecky() { };
    void fill_values(std::istream& s);
    void process(char c);
    void print_values();
    void clear_temp();
private:   
    int words { 0 };
    int chars { 0 };
    int sentences { 0 };
    int lines { 0 };
    int nums { 0 };
    int sum { 0 };

    bool inside_num { false };
    bool inside_word { false };
    int words_inside_sentence { 0 };
    int current_num { 0 };
    bool nonempty_line { false };
};

#endif
