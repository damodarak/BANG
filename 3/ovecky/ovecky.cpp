#include <iostream>
#include <cctype>
#include "ovecky.h"
using namespace std;

void Ovecky::fill_values(std::istream& s) 
{
    char c;

    for (;;) {
        c = s.get();

        if (s.fail())
        {
            if (nonempty_line)
            {
                lines++;
            }
            return;
        }
        else
        {
            process(c);
        }
    }
}

void Ovecky::process(char c)
{
    ++chars;
    if (c == '\n' && nonempty_line)
    {
        ++lines;
        inside_word = false;
        nonempty_line = false;
        if (inside_num)
        {
            sum += current_num;
            current_num = 0;
            inside_num = false;
        }
    }
    else if (c == '.' || c == '?' || c == '!')
    {
        if (inside_num)
        {
            sum += current_num;
            current_num = 0;
            inside_num = false;
        }
        if (inside_word)
        {
            ++sentences;
            inside_word = false;
            words_inside_sentence = 0;
        }
        else if (words_inside_sentence > 0)
        {
            ++sentences;
            words_inside_sentence = 0;
        }
    }
    else if(inside_word && !isalnum(c))
    {
        inside_word = false;
    }
    else if (inside_num)
    {
        if (isdigit(c))
        {
            current_num *= 10;
            current_num += c - '0';
        }
        else if(isalpha(c))
        {
            inside_num = false;
            inside_word = true;
            sum += current_num;
            current_num = 0;
        }
        else
        {
            inside_num = false;
            sum += current_num;
            current_num = 0;
        }
    }
    else
    {
        if (!inside_word && isalpha(c))
        {
            inside_word = true;
            ++words;
            ++words_inside_sentence;
            nonempty_line = true;
        }
        else if (!inside_num && isdigit(c))
        {
            inside_num = true;
            nums++;
            current_num = c - '0';
            nonempty_line = true;
        }
    }
}

void Ovecky::print_values() 
{
    cout << "znaku: " << chars << endl;
    cout << "slov: " << words << endl;
    cout << "vet: " << sentences << endl;
    cout << "radku: " << lines  << endl;
    cout << "cisel: " << nums << endl;
    cout << "soucet: " << sum << endl;
}
void Ovecky::clear_temp() 
{
    inside_num = false;
    inside_word = false;
    words_inside_sentence = 0;
    current_num = 0;
    nonempty_line = 0;
}
