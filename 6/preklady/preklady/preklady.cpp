#include <iostream>
#include <iterator>
#include <string>
#include <set>
#include <cctype>
#include <vector>

using namespace std;
using Dvojka = pair<string, string>;
using Nalezeni = vector<set<Dvojka>::iterator>;

struct cmp {
	bool operator()(const Dvojka& left, const Dvojka& right) const
	{
		if (left.first == right.first)
		{
			if (left.second.length() < right.second.length())
			{
				return true;
			}
			else if (left.second.length() == right.second.length())
			{
				return left.second < right.second;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return left.first < right.first;
		}
	}
};

class Preklady {
public:
	void add(const string& slovo, const string& preklad)
	{
		preklady.insert(Dvojka(slovo, preklad));
	}
	void del(const string& slovo, const string& preklad)
	{
		preklady.erase(Dvojka(slovo, preklad));
	}
	void del(const string& slovo)
	{
		set<Dvojka>::iterator it1 = preklady.end();
		set<Dvojka>::iterator it2 = preklady.end();
		bool found = false;

		for (auto i = preklady.begin(); i != preklady.end(); ++i)
		{
			if (!found && i->first == slovo)
			{
				found = true;
				it1 = i;
			}
			else if (found && i->first != slovo)
			{
				it2 = i;
				break;
			}
		}
		if (it1 != preklady.end())
		{
			preklady.erase(it1, it2);
		}	
	}
	Nalezeni find(const string& slovo)
	{
		set<Dvojka>::iterator it1 = preklady.end();
		set<Dvojka>::iterator it2 = preklady.end();
		bool found = false;

		for (auto i = preklady.begin(); i != preklady.end(); ++i)
		{
			if (!found && i->first == slovo)
			{
				found = true;
				it1 = i;
			}
			else if (found && i->first != slovo)
			{
				it2 = i;
				break;
			}
		}
		Nalezeni vec;
		vec.push_back(it1);
		vec.push_back(it2);
		return vec;
	}
	Nalezeni prefix(const string& pre)
	{
		set<Dvojka>::iterator it1 = preklady.end();
		set<Dvojka>::iterator it2 = preklady.end();
		bool found = false;

		for (auto i = preklady.begin(); i != preklady.end(); ++i)
		{
			string str = i->first;

			int size = pre.size();
			for (int i = 0; i < size; i++)
			{
				tolower(str[i]);
			}
			if (!found && str.compare(0, size, pre) == 0)
			{
				found = true;
				it1 = i;
			}
			else if (found && str.compare(0, size, pre) != 0)
			{
				it2 = i;
				break;
			}
		}
		Nalezeni vec;
		vec.push_back(it1);
		vec.push_back(it2);
		return vec;
	}
private:
	set<Dvojka, cmp> preklady;
};

void print(const Nalezeni& interval)
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
void print_pre(const Nalezeni& interval)
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

int main(int argc, char** argv) {
	Preklady p;
	string command;
	string s1;
	string s2;

	cin >> command;
	if (cin.fail())
	{
		return 0;
	}
	while (true)
	{
		if (command == "add")
		{
			cin >> s1;
			cin >> s2;
			p.add(s1, s2);
		}
		else if (command == "del")
		{
			cin >> s1;
			cin >> s2;
			if (s2 == "add" || s2 == "del" || s2 == "find" || s2 == "prefix")
			{
				command = s2;
				p.del(s1);

				continue;
			}
			else
			{
				p.del(s1, s2);
			}
		}
		else if (command == "find")
		{
			cin >> s1;
			Nalezeni vec;
			vec = p.find(s1);
			print(vec);
		}
		else if (command == "prefix")
		{
			cin >> s1;
			int size = s1.size();
			for (int i = 0; i < size; i++)
			{
				tolower(s1[i]);
			}
			Nalezeni vec;
			vec = p.prefix(s1);
			print_pre(vec);
		}

		cin >> command;
		if (cin.fail())
		{
			return 0;
		}
	}
}