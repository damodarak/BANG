#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>

using namespace std;

bool greater9(int value)
{
    return value > 9;
}

void inc(int& x)
{
    static int a = 0;
    x += (++a);
}

void inc1(int& x)
{
    x++;
}

void print(int x)
{
    cout << x << " ";
}

class ftor {
public:
    ftor(int step) : step_(step), a(0) {}
    void operator() (int& x)
    {
        x += (a += step_);
    }
private:
    int step_;
    int a;
};

class cmp {
public:
    cmp(int x) : a(x) {}
    bool operator() (int& x)
    {
        return x > a;
    }
private:
    int a;
};

class scitacka {
public:
    scitacka(int up) : up_(up), sum_(0) {}
    void operator()(int& x)
    {
        if (x > up_) {
            sum_ += x;
        }      
    }
    int sum_;
private:
    int up_;
};

class v2set {
public:
    v2set(int x, int y) : x_(x), y_(y) {};
    void operator()(int& x)
    {
        if (x > x_) {
            ms.insert(x + y_);
        }
    }
    multiset<int> mset()
    {
        return ms;
    }
private:
    multiset<int> ms;
    int x_;
    int y_;
};

int main()
{
    vector<int> v{ 1,2,3,4,5,6,7,11 };
    vector<int>::const_iterator ci = find(v.cbegin(), v.cend(), 5);
    cout << *ci << endl;


    auto it = find_if(v.cbegin(), v.cend(), &greater9);//pointer na funkci
    cout << *it << endl;

    for_each(v.begin(), v.end(), inc);
    for_each(v.begin(), v.end(), print);
    cout << endl;

    for_each(v.rbegin(), v.rend(), inc1);//reverse begin, reverse end, jde to od konce k zacatku
    for_each(v.begin(), v.end(), print);
    cout << endl;

    for_each(v.begin(), v.end(), ftor(2));
    for_each(v.begin(), v.end(), print);
    cout << endl;

    vector<int>::const_iterator cit = find_if(v.begin(), v.end(), cmp(30));
    cout << ((cit == v.end()) ? -1 : *cit) << endl;

    cit = find_if(v.begin(), v.end(), [](int& x) { return x > 30; });
    cout << ((cit == v.end()) ? -1 : *cit) << endl;

    auto s = for_each(v.begin(), v.end(), scitacka(20));
    cout << s.sum_ << endl;

    auto ss = for_each(v.begin(), v.end(), v2set(25, 1));
    multiset<int> ms = ss.mset();
    for_each(ms.begin(), ms.end(), print);
    cout << endl;

    return 0;
}