//David Kroupa
//du2-polysort.h

#ifndef DU2_POLYSORT_H
#define DU2_POLYSORT_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <memory>
#include <fstream>
#include <cctype>
#include <map>

class AbstractValue {
public:
    virtual void print(std::ostream& s) = 0;
    virtual bool operator>(const AbstractValue& second) const = 0;
};

using ValuePointer = std::unique_ptr<AbstractValue>;

class IntValue : public AbstractValue {
public:
    IntValue(int value) : value_(value) {}

    virtual void print(std::ostream& s) {
        s << value_;
    }

    virtual bool operator>(const AbstractValue& second) const override {
        const IntValue* second_value = dynamic_cast<const IntValue*>(&second);
        if (second_value == nullptr) {
            throw std::invalid_argument("Error");
        }

        return value_ > second_value->value_;
    }
private:
    int value_;
};

class StringValue : public AbstractValue {
public:
    StringValue(std::string value) : value_(value) {}

    virtual void print(std::ostream& s) {
        s << value_;
    }
    std::string val() {
        return value_;
    }

    virtual bool operator>(const AbstractValue& second) const override {
        const StringValue* second_value = dynamic_cast<const StringValue*>(&second);
        if (second_value == nullptr) {
            throw std::invalid_argument("Error");
        }

        return value_ > second_value->value_;
    }
private:
    std::string value_;
};

class Table {
public:
    Table() {
        new_line();
    }

    void new_line() {
        std::vector<ValuePointer> new_vec;
        matrix.push_back(move(new_vec));
    }

    bool add(std::string& str);
    void print();
    bool process_args(const std::vector<std::string>& args);
    void load_input(); 
    void sort();
    bool check();

private:
    std::string buffer = "";
    std::vector<std::vector<ValuePointer>> matrix;
    std::string input = "";
    std::string output = "";
    char sep = ' ';
    std::vector<std::string> col_sort;
    std::map<int, char> col_type;

    void print_matrix(std::ostream& s);
    bool is_num(const std::string& str);
    void process(char c, bool last);
    void process_input(std::istream& s);
    void sort_by_col(int col);
};


#endif
