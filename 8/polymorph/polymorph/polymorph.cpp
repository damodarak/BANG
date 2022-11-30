#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

class AbstractVal {
public:
    virtual void print() = 0;
    /*virtual unique_ptr<AbstractVal> clone() = 0;*/
    enum T { T_INT, T_STRING };
    virtual T get_t() = 0;
};

using Valptr = unique_ptr<AbstractVal>;

class IntVal : public AbstractVal
{
public:
    IntVal(int x) : x_(x) {};
    IntVal(const IntVal& tv) : x_(tv.x_) {};
    virtual T get_t() override
    {
        return T_INT;
    }
    virtual void print() override
    {
        cout << x_ << endl;
    }
    /*virtual T get_t() override
    {
        return AbstractVal::T_INT;
    }*/
   /* virtual Valptr clone() override
    {
        return make_unique<IntVal>(*this);
    }*/
private:
    int x_;
};

class StrVal : public AbstractVal
{
public:
    StrVal(string x) : x_(x) {};
    virtual T get_t() override
    {
        return T_STRING;
    }
    virtual void print() override
    {
        cout << x_ << endl;
    }
    /*virtual T get_t() override
    {
        return AbstractVal::T_STRING;
    }*/
    /*virtual Valptr clone() override
    {
        return make_unique<StrVal>(*this);
    }*/
private:
    string x_;
};

class Seznam {
public:
    void add(Valptr& p)
    {
        data.push_back(move(p));
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
       /* for (auto&& x : s.data)
            data.push_back(make_unique<IntVal>(*x));
        return *this;*/


    }
private:
    vector<Valptr> data;
};
    
int main()
{
    Seznam s;
    Valptr vp = make_unique<IntVal>(123);
    IntVal iv = vp->
    //IntVal iv = *vp;
    //Valptr vp1 = make_unique<IntVal>(*vp);
    //s.add(vp);
    //s.add(make_unique<StrVal>("456"));
    s.print();
    //Seznam s1 = s;
}
