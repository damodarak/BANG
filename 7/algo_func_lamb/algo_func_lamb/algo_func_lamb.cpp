#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int inc(int i) { return ++i; }

int main()
{
    vector<int> v = { 1,2,3,4,3,4,5 };
    multiset<int> ms = {};

    transform(v.begin(), v.end(), ms.begin(), inc);

    for (auto&& x : ms)
    {
        cout << x << endl;
    }
}