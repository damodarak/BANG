#include <iostream>
#include <iterator>
#include <string>
#include <set>
#include <map>
#include <unordered_set>

using namespace std;

class Slovnik {
public:
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
private:
	multimap<string, string> slovnik;
};

int main() {
	Slovnik sl;
	sl.add("a", "c");
	sl.add("a", "c");
	sl.add("a", "b");
	sl.add("a", "d");
	sl.add("b", "a");
	sl.del("a", "b");

	unordered_set<string> us = sl.find("a");
	for (auto&& x : us)
		cout << x << endl;
}