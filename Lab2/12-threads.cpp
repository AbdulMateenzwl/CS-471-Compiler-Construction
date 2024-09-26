#include <iostream>
#include <thread>
using namespace std;

void printFirstHalf()
{
    for (int i = 1; i <= 5; ++i)
    {
        cout << " " << i << " ";
    }
}

void printSecondHalf()
{
    for (int i = 6; i <= 10; ++i)
    {
        cout << " " << i << " ";
    }
}

int main()
{
    thread t1(printFirstHalf);
    thread t2(printSecondHalf);

    t1.join();
    t2.join();

    cout << endl
         << "Both threads finished execution." << endl;

    return 0;
}
