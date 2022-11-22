#include <iostream>
#include <iterator>
#include <vector>
#include <array>
#include "preklady.h"
#include <algorithm>
#include <cctype>

using namespace std;

bool cmp::operator()(const Dvojka& left, const Dvojka& right) const
{
	{
		if (left.first == right.first)
		{
			if (left.second.length() < right.second.length())
			{
				return true;
			}
			else if (left.second.length() == right.second.length())
			{
				string l2 = left.second;
				string r2 = right.second;
				for (auto& c : l2)
				{
					c = tolower(c);
				}
				for (auto& c : r2)
				{
					c = tolower(c);
				}
				return l2 < r2;
			}
			else
			{
				return false;
			}
		}
		else
		{
			string l1 = left.first;
			string r1 = right.first;
			for (auto& c : l1)
			{
				c = tolower(c);
			}
			for (auto& c : r1)
			{
				c = tolower(c);
			}


			if (l1 == r1)
			{
				return left.first < right.first;
			}
			else
			{
				return l1 < r1;
			}	
		}
	}
}
bool cmp1::operator()(const string& s1, const string& s2) const
{
	if (s1.length() < s2.length())
	{
		return true;
	}
	else if (s1.length() == s2.length())
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
		return l2 < r2;
	}
	else
	{
		return false;
	}
}
bool cmp2::operator()(const string& s1, const string& s2) const
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
	return l2 < r2;
}
bool mysort(const string& s1, const string& s2)
{
	if (s1.length() < s2.length())
	{
		return true;
	}
	else if (s1.length() == s2.length())
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
		return l2 < r2;
	}
	else
	{
		return false;
	}
}

void Preklady::add(const string& slovo, const string& preklad)
{
	preklady.insert(Dvojka(slovo, preklad));
	if (slova.find(slovo)==slova.end())
	{
		slova[slovo] = set{ preklad };
	}
	else
	{
		slova[slovo].insert(preklad);
	}

	string s = "";	
	int size = slovo.size();
	for (int i = 0; i < size; i++)
	{
		s += tolower(slovo[i]);
		if (prefixy.find(s) == prefixy.end())
		{
			prefixy[s] = set{ slovo };
		}
		else
		{
			prefixy[s].insert(slovo);
		}
	}
}

void Preklady::del(const string& slovo, const string& preklad)
{
	preklady.erase(Dvojka(slovo, preklad));
	slova[slovo].erase(preklad);


	string in = slovo;
	for (auto& c : in)
	{
		c = tolower(c);
	}
	if (slova[slovo].size() == 0 && prefixy.find(in) != prefixy.end()) {
		string s = "";
		int size = slovo.size();
		for (int i = 0; i < size; i++)
		{
			s += tolower(slovo[i]);
			prefixy[s].erase(slovo);
		}
		prefixy.erase(s);
	}	
}

void Preklady::del(const string& slovo)
{
	if (slova.find(slovo)!=slova.end())
	{
		vector<string> v;
		for (auto i = slova[slovo].begin(); i != slova[slovo].end(); i++)
		{
			v.push_back(slovo);
			v.push_back(*i);
		}
		int size = v.size() / 2;
		for (int i = 0; i < size; i++)
		{
			del(v[i*2], v[(i*2)+1]);
		}
		slova.erase(slovo);
	}
}

Rozmezi Preklady::find(const string& slovo)
{
	auto it1 = preklady.end();
	auto it2 = preklady.end();

	if (slova.find(slovo) != slova.end())
	{
		it1 = preklady.find(Dvojka(slovo, *slova[slovo].begin()));
		
		cmp1 compare;
		string nejmensi = it1->second;
		for (auto i = slova[slovo].begin(); i != slova[slovo].end(); i++) {
			if (compare(*i, nejmensi))
			{
				nejmensi = *i;
			}
		}
		it1 = preklady.find(Dvojka(slovo, nejmensi));
	}

	for (auto i = it1; i != preklady.end(); ++i)
	{
		if (i->first != slovo)
		{
			it2 = i;
			break;
		}
	}

	return { it1,it2 };
}
Rozmezi Preklady::prefix(const string& pre)
{
	string prvni = *prefixy.find(pre)->second.begin();
	string druhe = *slova[prvni].begin();

	cmp1 compare;
	for (auto i = slova.find(prvni)->second.begin(); i != slova.find(prvni)->second.end(); i++) {
		if (compare(*i, druhe))
		{
			druhe = *i;
		}
	}
	
	auto it1 = preklady.find(Dvojka(prvni, druhe));
	auto it2 = preklady.end();

	for (auto i = it1; i != preklady.end(); i++)
	{
		string str = i->first;
		int size = pre.size();
		for (auto& c : str)
		{
			c = tolower(c);
		}

		if (str.compare(0, size, pre) != 0)
		{
			it2 = i;
			break;
		}
	}
	return { it1,it2 };
}

void print(const Rozmezi& interval)
{
	if (interval[0] == interval[1])
	{
		return;
	}
	for (auto i = interval[0]; i != interval[1]; ++i)
	{
		cout << i->second << " ";
	}
	cout << endl;
}

void print_pre(const Rozmezi& interval)
{
	if (interval[0] == interval[1])
	{
		return;
	}

	string slovo = interval[0]->first;
	cout << slovo + ": ";

	for (auto i = interval[0]; i != interval[1]; ++i)
	{
		if (i->first == slovo)
		{
			cout << i->second + " ";
		}
		else
		{
			cout << endl;
			slovo = i->first;
			cout << slovo + ": " << i->second + " ";
		}
	}
	cout << endl;
}