#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int f(int& value)
{
    int y = 100;
    return value + y;
}

int main()
{
    vector<int> v = { 1,2,3,4};
    v.erase(remove_if(v.begin(), v.end(), [](int num) {return !(num>2);}));

    multiset<int> ms;
    transform(v.begin(), v.end(), inserter(ms, ms.begin()), &f);


    for (auto&& x : ms)
    {
        cout << x << endl;
    }
}