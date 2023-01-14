//David Kroupa
//du2-polysort.cpp

#include "du2-polysort.h"
#include <algorithm>

using namespace std;

bool Table::add(string& str) {
    int lines = matrix.size() - 1;
    int col = matrix[lines].size();

    if (col_type.find(col) == col_type.end()) {
        ValuePointer vp = make_unique<StringValue>(str);
        matrix[lines].push_back(move(vp));
    }
    else {
        char type = col_type.find(col)->second;
        if (type == 'S') {
            ValuePointer vp = make_unique<StringValue>(str);
            matrix[lines].push_back(move(vp));
        }
        else if(type == 'N' && is_num(str)){
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
        for (long unsigned i = 0; i < row.size() - 1; ++i) {
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
bool Table::process_args(const vector<string>& args) {
    for (long unsigned i = 1; i < args.size(); ++i) {
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
        else if ((args[i].substr(0, 1) == "N" || args[i].substr(0, 1) == "S") && args[i].size() > 1) {
            col_sort.push_back(args[i]);
            col_type[stoi(args[i].substr(1)) - 1] = args[i].substr(0, 1)[0];
        }
        else {
            cerr << "Error in command line arguments, " << args[i] << " was not expected" << endl;
            return false;
        }
    }
    return true;
}
void Table::load_input() {
    if (input == "") {
        process_input(cin);
        if (matrix[matrix.size() - 1].size() == 0) {
            matrix.pop_back();
        }
    }
    else {
        ifstream f;
        f.open(input);
        if (f.good()) {
            process_input(f);
            if (matrix[matrix.size() - 1].size() == 0) {
                matrix.pop_back();
            }
        }
        f.close();
    }
}
bool Table::process(char c, bool last) {
    if (last) {
        if (matrix[0].size() > matrix[matrix.size() - 1].size() && matrix[matrix.size() - 1].size() != 0) {
            add(buffer);
        }
        return true;
    }
    
    bool res = true;
    if (c == '\n' || c =='\r') {
        res = add(buffer);
        new_line();
        buffer = "";
    }
    else if (c == sep) {
        res = add(buffer);
        buffer = "";
    }
    else {
        buffer += c;
    }
    return res;
}
void Table::process_input(istream& s) {
    char c;
    for (;;) {
        c = s.get();

        if (s.fail())
        {
            process(c, true);
            return;
        }
        else
        {
            bool res = process(c, false);
            if (!res) {
                fprintf(stderr, "error: radka %d, sloupec %d - nepripustny format", line, column);
                error = true;
                return;
            }
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
void Table::sort() {
    while (!col_sort.empty()) {
        sort_by_col(stoi(col_sort.back().substr(1)) - 1);
        col_sort.pop_back();
    }
}
void Table::sort_by_col(int col) {
    std::sort(matrix.begin(), matrix.end(), [&](const vector<ValuePointer>& line1, const vector<ValuePointer>& line2) 
        { 
            return line2[col]->operator>(*line1[col]); 
        });
}
bool Table::check() {
    if (error) {
        return false;
    }
    for (auto it = col_type.begin(); it != col_type.end(); it++)
    {
        for (auto&& row : matrix) {
            
            if (uint64_t(it->first) > row.size()) {
                cerr << "Error in argument" << endl;
                return false;
            }
        }       
    }
    return true;
}
