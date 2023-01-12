//David Kroupa
//du2-polysort.h

#ifndef DU2_POLYSORT_H
#define DU2_POLYSORT_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <memory>

class AbstractValue {
public:
    virtual void print(std::ostream& s) = 0;
    virtual std::unique_ptr<AbstractValue> clone() = 0;
    virtual bool operator>(const AbstractValue& second) const = 0;
};

using ValuePointer = std::unique_ptr<AbstractValue>;

class IntValue : public AbstractValue {
public:
    IntValue(int value) : value_(value) {}

    virtual ValuePointer clone() override {
        return std::make_unique<IntValue>(*this);
    }

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

    virtual ValuePointer clone() override {
        return std::make_unique<StringValue>(*this);
    }

    virtual void print(std::ostream& s) {
        s << value_;
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
    void add(ValuePointer p) {
        matrix[matrix.size() - 1].push_back(std::move(p));
    };
    void new_line() {
        std::vector<ValuePointer> new_vec;
        matrix.push_back(new_vec);
    }

    void print(std::ostream& s) {
        for (auto&& row : matrix) {
            for (int i = 0; i < row.size() - 1; ++i) {
                row[i]->print(s);
                s << sep;
            }
            row[row.size() - 1]->print(s);
        }
        s << std::endl;
    }

    void process_args(std::vector<std::string>& args);
private:
    std::vector<std::vector<ValuePointer>> matrix;
    std::string input = "";
    std::string output = "";
    char sep = ' ';
    std::vector<std::string> col_sort;
};



#endif