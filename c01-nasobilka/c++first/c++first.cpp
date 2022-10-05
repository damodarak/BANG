#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Nasobilka {
public:
    Nasobilka(const vector<string>& s)
    {
        checkStart(s);
    }

    void PrintTable(const vector<string>& s)
    {
        for (int i = 1; i < s.size(); i++)
        {
            if (s[i] == "-f" || s[i] == "-t")
            {
                i++;
            }
            else
            {
                printMulti(stoi(s[i]));
            }
        }
    }
private:
    int start;
    int end;
    void checkStart(const vector<string>& s)
    {
        start = 0;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i]=="-f")
            {
                start = stoi(s[i + 1]);
            }
        }

        end = 10;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == "-t")
            {
                end = stoi(s[i + 1]);
            }
        }       
    }
    void printMulti(int num)
    {
        for (int i = start; i < end + 1; i++)
        {
            cout << i << " * " << num << " = " << i * num << endl;
        }
    }
};

int main( int argc, char ** argv)
{
    vector<string> arg(argv, argv + argc);
    Nasobilka nasob(arg);
    nasob.PrintTable(arg);

    return 0;
}
