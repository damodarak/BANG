#include <iostream>
#include <cctype>
#include "preklady.h"

using namespace std;

bool cmplen::operator()(const string& s1, const string& s2) const
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
bool cmpnorm::operator()(const string& s1, const string& s2) const
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

void Preklady::add(const string& slovo, const string& preklad)
{
	if (slova.find(slovo) == slova.end())
	{
		slova.insert({ slovo, {preklad} });
	}
	else
	{
		slova.find(slovo)->second.insert(preklad);
	}
}

void Preklady::del(const string& slovo, const string& preklad)
{
	if (slova.end() != slova.find(slovo))
	{
		slova.find(slovo)->second.erase(preklad);
		if (slova.find(slovo)->second.size() == 0)
		{
			slova.erase(slovo);
		}
	}
}

void Preklady::del(const string& slovo)
{
	slova.erase(slovo);
}

Mapa::const_iterator Preklady::find(const string& slovo)
{
	Mapa::const_iterator it = slova.find(slovo);
	return it;
}
Dvojka Preklady::prefix(const string& pre)
{
	Mapa::const_iterator it1 = slova.lower_bound(pre);
	if (pre.empty() || it1 == slova.end())
	{
		return { slova.end(), slova.end() };
	}

	string pre2 = pre;
	char& c = pre2[pre2.size() - 1];
	c++;
	Mapa::const_iterator it2 = slova.lower_bound(pre2);
	return { it1, it2 };
}

void Preklady::print(const Mapa::const_iterator& it)
{
	if (slova.end() == it)
	{
		return;
	}
	for (Mnoz::iterator iter = it->second.begin(); iter != it->second.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;
}

void Preklady::print_pre(const Dvojka& dv)
{
	if (dv.first == slova.end())
	{
		return;
	}
	else
	{
		for (Mapa::const_iterator it = dv.first; it != dv.second; it++)
		{
			cout << it->first + ": ";
			print(it);
		}
	}
}