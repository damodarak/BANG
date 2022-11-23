#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

struct cmpm {
    bool operator()(const string& s1, const string& s2) const
    {
        if (s1.length() == s2.length())
        {
            return s1 < s2;
        }
        else
        {
            return s1.length() < s2.length();
        }
    }
};

struct cmps {
    bool operator()(const string& s1, const string& s2) const
    {
        if (s1.length() == s2.length())
        {
            return s1 < s2;
        }
        else
        {
            return s1.length() < s2.length();
        }
    }
};


int main()
{
    map<string, int, cmpm> m;
    m.insert({ "aa",1 });
    m.insert({ "b",2 });

    map<string, set<string, cmps>> mm;
    mm.insert({ "a", {"a"} });
    mm["a"].insert("b");

}