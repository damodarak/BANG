#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ovecky.h"
using namespace std;

int main( int argc, char** argv)
{
	vector<string> arg(argv, argv + argc);
	Ovecky ov;
	if (arg.size()>1)
	{
		ifstream f;
		int size = arg.size();
		for (int i = 1; i < size; i++)
		{
			f.open(arg[i]);
			ov.fill_values(f);
			ov.clear_temp();
			f.close();
		}
	}
	else
	{
		ov.fill_values(std::cin);
	}	
	ov.print_values();
}