#include <iostream>
#include <list>
using namespace std;
int main()
{
    list<int> pages;
    auto pointer = pages.begin();

    while (true)
    {
        int option = 0;
        cout << endl;
        cout << "1- Enter page" << endl;
        cout << "2- Move Back" << endl;
        cout << "3- Move Forward" << endl;
        cout << "4- Print Current Page" << endl;

        cin >> option;

        if (option == 1)
        {
            pages.push_back(pages.size() + 1);
            ++pointer;
        }
        else if (option == 2)
        {
            if (pointer != pages.begin())
            {
                pointer--;
            }
            else
            {
                cout << "You are at the beginning of the list" << endl;
            }
        }
        else if (option == 3)
        {
            if (pointer != pages.end())
            {
                pointer++;
            }
            else
            {
                cout << "You are at the end of the list" << endl;
            }
        }
        else if (option == 4)
        {
            cout << "Current page: " << *pointer << endl;
        }
        else
        {
            break;
        }
    }
}