#include <iostream>
#include <string>

using namespace std;

template<typename T> class Zlomek
{
public:
    Zlomek<T>(T hore, T dole) : numerator(hore), denominator(dole) {};
    void add(T hore, T dole)
    {
        numerator = dole * numerator + hore * denominator;
        denominator = denominator * dole;    
    }
    string result()
    {
        return to_string(numerator) + "/" + to_string(denominator);
    }
    template<typename X>
    friend Zlomek<X> operator+(const Zlomek<X>& x, const Zlomek<X>& y);
private:
    T numerator;
    T denominator;
};

template<typename X>
Zlomek<X> operator+(const Zlomek<X>& x, const Zlomek<X>& y)
{
    return Zlomek<X> { x.numerator * y.denominator + y.numerator * x.denominator, x.denominator + y.denominator};
}


int main()
{
    Zlomek<long> zl(1,1);
    zl.add(1, 1);
    Zlomek<long> a = zl;
    Zlomek<long> b = zl + a;
 
    cout << b.result();

}