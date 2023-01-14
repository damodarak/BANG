//David Kroupa
//du2-polysort.cpp

#include "du2-polysort.h"

using namespace std;

bool Table::add(string& str) {
    int lines = matrix.size() - 1;
    int col = matrix[lines].size();

    if (col_type.find(col) == col_type.end()) {
        if (is_num(str)) {
            ValuePointer vp = make_unique<IntValue>(stoi(str));
            matrix[lines].push_back(move(vp));
        }
        else {
            ValuePointer vp = make_unique<StringValue>(str);
            matrix[lines].push_back(move(vp));
        }
    }
    else {
        char type = col_type.find(col)->second;
        if (type == 'S') {
            ValuePointer vp = make_unique<StringValue>(str);
            matrix[lines].push_back(move(vp));
        }
        else if(is_num(str)){
            ValuePointer vp = make_unique<IntValue>(stoi(str));
            matrix[lines].push_back(move(vp));
        }
        else {
            return false;
        }
    }
    return true;
};
void Table::print_matrix(ostream& s) {
    for (auto&& row : matrix) {
        for (int i = 0; i < row.size() - 1; ++i) {
            row[i]->print(s);
            s << sep;
        }
        row[row.size() - 1]->print(s);
        s << endl;
    }
    s << endl;
}
void Table::print() {
    if (output == "") {
        print_matrix(cout);
    }
    else {
        ofstream f(output);
        print_matrix(f);
        f.close();
    }
}
void Table::process_args(const vector<string>& args) {
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
        else if (args[i] == "polysort") {
            continue;
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
        else if ((args[i].substr(0, 1) == "N" || args[i].substr(0, 1) == "S") && args[i].size() > 1) {
            col_sort.push_back(args[i]);
            col_type[stoi(args[i].substr(1))] = args[i].substr(0, 1)[0];
        }
        else {
            /*cerr << "Error in command line arguments, " << *iter << " was not expected" << endl;
            there_is_error = true;
            return;*/
            //dodelat
        }
    }
}
void Table::load_input() {
    if (input == "") {
        //return process_input(cin) ? true : false;
        process_input(cin);
    }
    else {
        ifstream f;
        f.open(input);
        if (f.good()) {
            //bool result = process_input(f) ? true : false;
            process_input(f);
            matrix.pop_back();
            f.close();
            return;
        }
        else {
            f.close();
            return;
        }
    }
}
void Table::process(char c) {
    if (c == '\n') {
        add(buffer);
        new_line();
        buffer = "";
    }
    else if (c == sep) {
        add(buffer);
        buffer = "";
    }
    else {
        buffer += c;
    }
}
void Table::process_input(istream& s) {
    char c;
    for (;;) {
        c = s.get();

        if (s.fail())
        {
            return;
        }
        else
        {
            process(c);
        }
    }
}
bool Table::is_num(const string& str) {
    if (str == "") {
        return false;
    }

    for (auto&& c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
