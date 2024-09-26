#include <iostream>
#include <deque>
using namespace std;
main()
{
    deque<string> dq;
    while (true)
    {
        cout << "1- Insert on Front" << endl;
        cout << "2- Insert on End" << endl;
        cout << "3- Remove on Front" << endl;
        cout << "4- Remove on End" << endl;
        cout << "5- Print Top Task" << endl;
        cout << "6- Print Low Task" << endl;
        cout << "7- Print tasks" << endl;

        int option;

        cin >> option;

        if (option == 1)
        {
            string task;
            cout << "Enter Task: ";
            cin >> task;
            dq.push_front(task);
        }
        else if (option == 2)
        {
            string task;
            cout << "Enter Task: ";
            cin >> task;
            dq.push_back(task);
        }
        else if (option == 3)
        {
            if (dq.size() > 0)
            {
                dq.pop_front();
            }
            else
            {
                cout << "Queue is empty" << endl;
            }
        }
        else if (option == 4)
        {
            if (dq.size() > 0)
            {
                dq.pop_back();
            }
            else
            {
                cout << "Queue is empty" << endl;
            }
        }
        else if (option == 5)
        {
            cout << "Top Task: " << dq.front() << endl;
        }
        else if (option == 6)
        {
            cout << "End Task: " << dq.back() << endl;
        }
        else if(option == 7){
            for (int i = 0; i < dq.size(); i++)
            {
                cout << dq[i] << endl;
            }
        }
        else
        {
            break;
        }
    }
}