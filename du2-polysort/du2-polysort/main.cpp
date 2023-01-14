//David Kroupa
//main.cpp

#include "du2-polysort.h"

using namespace std;

int main(int argc, char** argv)
{
	vector<string> arg(argv, argv + argc);
	Table t;

	t.process_args(arg);
	t.load_input();
	t.print();
	return 0;
}
