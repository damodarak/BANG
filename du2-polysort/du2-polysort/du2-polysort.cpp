//David Kroupa
//du2-polysort.h

#include "du2-polysort.h"

using namespace std;

void Table::process_args(vector<string>& args) {
    for (int i = 1; i < args.size(); ++i) {
        if (args[i].substr(0, 2) == "-i") {
            if (args[i].size() > 2) {
                input = args[i].substr(2);
            }
            else {
                i++;
                input = args[i];
            }
        }
        else if (args[i].substr(0, 2) == "-o") {
            if (args[i].size() > 2) {
                output = args[i].substr(2);
            }
            else {
                i++;
                output = args[i];
            }
        }
        else if (args[i].substr(0, 2) == "-s") {
            if (args[i].size() > 2) {
                sep = args[i][2];
            }
            else {
                i++;
                sep = args[i][0];
            }
        }
        else if (args[i].substr(0, 1) == "N" || args[i].substr(0, 1) == "S") {
            col_sort.push_back(args[i]);
        }
        else {
            /*cerr << "Error in command line arguments, " << *iter << " was not expected" << endl;
            there_is_error = true;
            return;*/
            //dodelat
        }
    }
}