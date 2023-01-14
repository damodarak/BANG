//David Kroupa
//main.cpp

#include "du2-polysort.h"

using namespace std;

int main(int argc, char** argv)
{
	vector<string> arg(argv, argv + argc);
	Table t;

	if (!t.process_args(arg)) {
		return 0;
	}
	t.load_input();
	if (!t.check()) {
		return 0;
	}
	t.sort();
	t.print();
	return 0;
}
