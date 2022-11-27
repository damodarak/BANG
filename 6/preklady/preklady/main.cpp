#include <iostream>
#include "preklady.h"

using namespace std;

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
			Mapa::const_iterator roz;
			roz = p.find(s1);
			p.print(roz);
		}
		else if (command == "prefix")
		{
			cin >> s1;
			for (auto& c : s1)
			{
				c = tolower(c);
			}

			Dvojka roz;
			roz = p.prefix(s1);
			p.print_pre(roz);
		}

		cin >> command;
		if (cin.fail())
		{
			return 0;
		}
	}
}