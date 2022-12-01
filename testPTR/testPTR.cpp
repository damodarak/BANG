#include <iostream>
#include <vector>

using namespace std;

class AbstractVal {
public:
    virtual void print() = 0;
    
};

using Valptr = unique_ptr<AbstractVal>;

class IntVal : public AbstractVal {
public:
    IntVal(int x) : x_(x) {}
    unique_ptr<AbstractVal> clone() {
        return make_unique<IntVal>(*this);
    }
    virtual void print() { cout << x_ << endl; }
private:
    int x_;
};

class StringVal : public AbstractVal {
public:
    StringVal(string x) : x_(x) {}
    virtual void print() { cout << x_ << endl; }
private:
    string x_;
};

class Seznam {
public:
    void add(Valptr p) { pole_.push_back(move(p)); }
    void print() { for (auto&& x : pole_) x->print(); }

    /*Seznam& operator=(const Seznam& s)
    {
        for (auto&& x : s.pole_) {
            switch (x->get_t()) {
            case AbstractVal::T_INT:
                pole_.push_back(make_unique<IntVal>(*x));
                break;
            case AbstractVal::T_STRING:
                pole_.push_back(make_unique<StringVal>(*x));
                break;
            }
            return *this;
        }
    }*/

private:
    vector<Valptr> pole_;
};


int main()
{
    /*unique_ptr<int> a = make_unique<int>(123);
    cout << *a;*/

    //Seznam s;
    Valptr a = make_unique<IntVal>(1111);
    Valptr b = make_unique<IntVal>(*a);



    /*s.add(make_unique<IntVal>(123));
    s.add(make_unique<StringVal>("456"));
    s.print();*/

}