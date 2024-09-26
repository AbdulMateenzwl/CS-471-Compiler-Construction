#include <iostream>
#include <vector>
using namespace std;
int menu()
{
    int option = 0;
    cout << endl;
    cout << "1- Enter Student data" << endl;
    cout << "2- Delete Student data" << endl;
    cout << "3- Print Student data" << endl;
    cin >> option;
    return option;
}
main()
{
    vector<int> vtr;
    while (true)
    {
        int option = menu();
        if (option == 1)
        {
            int numbers = 0;
            cout << "Enter student numbers : ";
            cin >> numbers;
            vtr.push_back(numbers);
        }
        else if (option == 2)
        {
            int index = -1;
            cout << "Enter Index to delete";
            cin >> index;
            if (index >= vtr.size() && index > 0)
            {
                cout << "Index out of range" << endl;
            }
            else
            {
                vtr.erase(std::next(vtr.begin()));
            }
        }
        else if (option == 3)
        {
            for (int i = 0; i < vtr.size(); i++)
            {
                cout << i << ") " << vtr[i] << endl;
            }
        }
        else
        {
            break;
        }
    }
}