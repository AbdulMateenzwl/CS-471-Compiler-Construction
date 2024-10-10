#include <iostream>
#include <set>
using namespace std;

set<int> sett;

bool find(int val)
{
    if (sett.find(val) == sett.end())
    {
        return false;
    }
    return true;
}

set<int> getIds()
{
    return sett;
}

void printIds(set<int> sett)
{
    for (int val : sett)
    {
        cout << val << "  ";
    }
}

bool add(int val)
{
    if (!find(val))
    {
        sett.insert(val);
        return true;
    }
    return false;
}

main()
{
    add(6);
    add(102);
    add(102);
    add(102);
    add(456);

    printIds(getIds());
    
    // cout<<find(102);
}