#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

class CountWord {
public:
    CountWord()
    {
        words = chars = sentences = lines = words_inside_sentence = 0;
        inside_word = false;
    }
    void fill_values(istream& s)
    {
        char c;

        for (;;) {
            c = s.get();

            if (s.fail())
            {
                return;
            }
            else
            {
                process(c);
            }
        }
    }
    void print_values()
    {
        cout << "Chars: " << chars << endl;
        cout << "Words: " << words << endl;
        cout << "Sentences: " << sentences << endl;
        cout << "Lines: " << lines << endl;
    }
private:
    void process( char c)
    {
        if (c == '\n')
        {
            ++lines;
            inside_word = false;
        }
        else if(c == '.' || c == '?' || c == '!')
        {
            ++chars;
            if (inside_word)
            {
                ++sentences;
                inside_word = false;
                words_inside_sentence = 0;
            }
            else if (words_inside_sentence>0)
            {
                ++sentences;
                words_inside_sentence = 0;
            }
        }
        else
        {
            bool check = isalnum(c);
            ++chars;
            if (!inside_word && check)
            {
                inside_word = true;
                ++words;
            }
            else if (inside_word && !check)
            {
                inside_word = false;
            }
        }
    }
    int words;
    int chars;
    int sentences;
    int lines;
    bool inside_word;
    int words_inside_sentence;
};

int main()
{
    ifstream f;
    f.open("lorem.txt");
    CountWord cw;
    if (f.good()) 
    {
        //cw.fill_values(cin);
        cw.fill_values(f);
        cw.print_values();
    }
    f.close();
}