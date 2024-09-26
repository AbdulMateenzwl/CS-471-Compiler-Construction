#include <iostream>
#include <set>
using namespace std;
main()
{
    set<string> emails;
    while (true)
    {
        cout << "1- Insert" << endl;
        cout << "2- Remove" << endl;
        cout << "3- Print" << endl;
        cout << "4- Search" << endl;
        cout << "5- Exit" << endl;
        int option;
        cin >> option;

        if (option == 1)
        {
            string email;
            cout << "Enter Email: ";
            cin >> email;
            emails.insert(email);
        }
        else if (option == 2)
        {
            string email;
            cout << "Enter Email: ";
            cin >> email;
            emails.erase(email);
        }
        else if (option == 3)
        {
            for (auto email : emails)
            {
                cout << email << endl;
            }
        }
        else if (option == 4)
        {
            string email;
            cout << "Enter Email: ";
            cin >> email;
            if (emails.find(email) != emails.end())
            {
                cout << "Email found" << endl;
            }
            else
            {
                cout << "Email not found" << endl;
            }
        }
        else
        {
            break;
        }
    }
}