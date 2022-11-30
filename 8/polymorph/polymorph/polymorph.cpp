#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

class AbstractVal {
public:
    virtual void print() = 0;

    using valptr = unique_ptr<AbstractVal>;
    virtual valptr clone() = 0;
};

using Valptr = unique_ptr<AbstractVal>;

class IntVal : public AbstractVal
{
public:
    IntVal(int x) : x_(x) {};   
    virtual void print() override
    {
        cout << x_ << endl;
    }
    virtual valptr clone() override
    {
        return make_unique<IntVal>(*this);
    }
private:
    int x_;
};

class StrVal : public AbstractVal
{
public:
    StrVal(string x) : x_(x) {};
    virtual void print() override
    {
        cout << x_ << endl;
    }
    virtual valptr clone() override
    {
        return make_unique<StrVal>(*this);
    }
private:
    string x_;
};

class Seznam {
public:
    void add(Valptr p)
    {
        data.push_back( move( p));
    }
    void print()
    {
        for (auto&& x : data)
        {
            x->print();
        }
    }
    Seznam& operator=(const Seznam& s)
    {
        for (auto&& x : s.data) {
            data.push_back(x->clone());
        }
        return *this;
    }
private:
    vector<Valptr> data;
};

int main()
{
    Seznam s;
    s.add(make_unique<IntVal>(123));
    s.add(make_unique<StrVal>("456"));
    s.print();
    Seznam s1 = s;
}
