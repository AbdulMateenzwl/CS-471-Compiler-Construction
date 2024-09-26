#include <iostream>
#include <queue>
using namespace std;
main()
{
    queue<string> personQ;
    queue<string> vipQ;
    while (true)
    {
        cout << "0- Issue Ticket" << endl;
        cout << "1- Insert VIP" << endl;
        cout << "2- Insert Person" << endl;
        cout << "3- Exit" << endl;

        int option;

        cin >> option;

        if (option == 0)
        {
            if (vipQ.size() == 0)
            {
                cout << "Ticket Issued: " << personQ.front() << endl;
                personQ.pop();
            }
            else
            {
                cout << "Ticket Issued: " << vipQ.front() << endl;
                vipQ.pop();
            }
        }
        if (option == 1)
        {
            string task;
            cout << "Enter VIP: ";
            cin >> task;
            vipQ.push(task);
        }
        else if (option == 2)
        {
            string task;
            cout << "Enter Person: ";
            cin >> task;
            personQ.push(task);
        }
        else{
            break;
        }
    }
}