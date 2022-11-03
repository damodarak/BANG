#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <vector>

using namespace std;

class Slovnik {
public:
	void read_text(istream & str) 
	{
		string s1;
		string s2;
		while (true)
		{
			str >> s1;
			str >> s2;
			if (str.fail())
			{
				break;
			}
			else
			{
				add(s1, s2);
			}
		}
	}
	unordered_set<string> find(const string& slovo)
	{
		auto itr1 = slovnik.lower_bound(slovo);
		auto itr2 = slovnik.upper_bound(slovo);

		unordered_set<string> us;
		while (itr1 != itr2)
		{
			if (itr1->first == slovo)
				us.insert(itr1->second);
			itr1++;
		}
		return us;
	}
	void del(const string& slovo, const string& cizi)
	{
		auto itr1 = slovnik.lower_bound(slovo);
		auto itr2 = slovnik.upper_bound(slovo);

		while (itr1 != itr2)
		{
			if (itr1->first == slovo && itr1->second == cizi)
			{
				slovnik.erase(itr1);
				return;
			}
			itr1++;
		}
	}
	void del(const string& slovo)
	{
		slovnik.erase(slovo);
	}
private:
	multimap<string, string> slovnik;
	void add(const string& slovo, const string& cizi)
	{
		auto itr1 = slovnik.lower_bound(slovo);
		auto itr2 = slovnik.upper_bound(slovo);
		bool found = false;

		while (itr1 != itr2)
		{
			if (itr1->first == slovo && itr1->second == cizi)
				found = true;
			itr1++;
		}
		if (!found)
		{
			slovnik.insert(pair<string, string>(slovo, cizi));
		}
	}
};

int main(int argc, char** argv) {
	Slovnik sl;
	/*
	sl.add("a", "c");
	sl.add("a", "c");
	sl.add("a", "b");
	sl.add("a", "d");
	sl.add("b", "a");
	sl.del("a", "b");
	*/
	vector<string> arg(argv, argv + argc);
	if (arg.size()>1)
	{
		ifstream f;
		int size = arg.size();
		for (int i = 1; i < size; i++)
		{
			f.open(arg[i]);
			sl.read_text(f);
			f.close();
		}
	}
	else
	{
		sl.read_text(cin);
	} 

	unordered_set<string> us = sl.find("a");
	for (auto&& x : us)
		cout << x << endl;
}