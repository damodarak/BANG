#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class Count {
public:
	void count_words(istream& str) 
	{
		string s;
		while (true)
		{
			str >> s;
			if (str.fail())
			{
				break;
			}
			else
			{
				auto i = m.find(s);
				if (i != m.end())
				{
					m[s] = i->second + 1;
				}
				else
				{
					m[s] = 1;
				}
			}
		}
	}
	void print_values()
	{
		for (auto&& [key, value] : m)
				cout << key << " : " << value << endl;
	}
private:
	map<string, int> m;
};

int main( int argc, char** argv)
{
	Count cnt;
	cnt.count_words(cin);
	cnt.print_values();
}
