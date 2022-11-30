#include <iostream>
#include <map>
#include <string>
#include <set>
#include <iterator>

using namespace std;

struct cmplen {
    bool operator()(const string& s1, const string& s2) const
    {
        if (s1.length() != s2.length())
        {
            return s1.length() < s2.length();
        }
        else
        {
            return s1 < s2;
        }
    }
};
struct cmpnorm {
    bool operator()(const string& s1, const string& s2) const
    {
        if (s1 != s2)
        {
            string l2 = s1;
            string r2 = s2;
            for (auto& c : l2)
            {
                c = tolower(c);
            }
            for (auto& c : r2)
            {
                c = tolower(c);
            }
            if (l2 == r2)
            {
                return s1 < s2;
            }
            else
            {
                return l2 < r2;
            }      
        }
        else
        {
            return false;
        }
    }
};

using Mnoz = set<string, cmplen>;
using Mapa = map<string, Mnoz, cmpnorm>;
using Dvojka = pair<Mapa::const_iterator, Mapa::const_iterator>;

void add( Mapa& m, const string& slovo, const string& preklad)
{
    if (m.find(slovo) == m.end())
    {
        m.insert({ slovo, {preklad} });
    }
    else
    {
        m.find(slovo)->second.insert(preklad);
    }
}
void del( Mapa& m, const string& slovo, const string& preklad)
{
    if (m.end() != m.find(slovo))
    {
        m.find(slovo)->second.erase(preklad);
        if (m.find(slovo)->second.size() == 0)
        {
            m.erase(slovo);
        }
    }
}
void del( Mapa& m, const string& slovo)
{
    m.erase(slovo);
}
Mapa::const_iterator find( const Mapa& m, const string& slovo)
{
    Mapa::const_iterator it = m.find(slovo);
    //Mapa::const_iterator it1 = m.lower_bound(slovo);
    return it;
}
Dvojka prefix(const Mapa& m, const string& pre)
{
    Mapa::const_iterator it1 = m.lower_bound(pre);
    if (pre.empty() || it1 == m.end())
    {
        return { m.end(), m.end() };
    }

    string pre2 = pre;
    char& c = pre2[pre2.size() - 1];
    c++;
    Mapa::const_iterator it2 = m.lower_bound(pre2);
    return { it1, it2 };
}
void print( const Mapa& m, const Mapa::const_iterator& it)
{
    if (m.end() == it)
    {
        return;
    }
    for (Mnoz::iterator iter = it->second.begin();  iter != it->second.end(); ++iter)
    {
        cout << *iter << " ";
    }
    cout << endl;
}
void print_pre( const Mapa& m, const Dvojka dv)
{
    if (dv.first == m.end())
    {
        return;
    }
    else
    {
        for (Mapa::const_iterator it = dv.first; it != dv.second; it++)
        {
            cout << it->first + ": ";
            print(m, it);
        }
    }
}

int main()
{
    Mapa m;

    add(m, "auto", "1");
    add(m, "aula", "2");
    add(m, "akro", "0");
    add(m, "beta", "3");
    Mapa::const_iterator ci = find(m, "b");
    Dvojka ci1 = prefix(m, "au");
    print_pre(m, ci1);
}