#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool findChar(char c, string specialChars)
{
    for (int i = 0; i < specialChars.length(); i++)
    {
        if (c == specialChars[i])
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    vector<int> data;
    data.push_back(1);
    data.push_back(2);

    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << endl;
    }
}