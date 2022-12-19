#include <iostream>
#include <vector>
#include <string>


class AbstractValue {
public:
    virtual void print() = 0;
    virtual std::unique_ptr<AbstractValue> clone() = 0;
};

using ValuePointer = std::unique_ptr<AbstractValue>;

class IntValue : public AbstractValue {
public:
    IntValue(int value) : value_(value) {}

    virtual ValuePointer clone() override {
        return std::make_unique<IntValue>(*this);
    }

    virtual void print() {
        std::cout << value_ << std::endl;
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

    virtual void print() {
        std::cout << value_ << std::endl;
    }
private:
    std::string value_;
};

class PolymorficList {
public:
    void add(ValuePointer p) {
        list_.push_back(std::move(p));
    };

    PolymorficList() {

    }

    PolymorficList(const PolymorficList& l) {
        clone(l);
    }

    PolymorficList& operator=(const PolymorficList& l) {
        if (this == &l) return *this;

        list_.clear();
        clone(l);
        return *this;
    }

    void print() {
        for (auto&& value : list_) {
            value->print();
        }
    }
private:
    std::vector<ValuePointer> list_;

    void clone(const PolymorficList& l) {
        for (auto&& x : l.list_) list_.push_back(x->clone());
    }
};