#include <iostream>
#include <fstream>

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
    if (argc < 2)
    {
        cout << "No file name argument is Provided" << endl;
        exit(1); // terminate with error
    }

    string fileName = argv[1];

    ifstream inFile;
    inFile.open(fileName);
    if (!inFile)
    {
        cout << "No file with name " << fileName << " found" << endl;
        exit(1); // terminate with error
    }

    string specialChars = "#{}[]()<>+-*/;.,&|!~^%=\"\'@?:&`$";

    string str;
    bool isMultiLineComment = false;
    while (getline(inFile, str))
    {
        int i = 0;
        string parsedString = "";
        bool isSingleLineComment = false;
        while (i < str.length())
        {
            if (isMultiLineComment)
            {
                if (str[i] == '*' && i + 1 < str.length() && str[i + 1] == '/')
                {
                    isMultiLineComment = false;
                    i += 2;
                }
                else
                {
                    i++;
                }
                continue;
            }
            if (isSingleLineComment)
            {
                if (str[i] == '\n')
                {
                    isSingleLineComment = false;
                }
                i++;
                continue;
            }
            if (str[i] == ' ')
            {
                if (parsedString != "")
                {
                    cout << parsedString << endl;
                }
                parsedString = "";
                i++;
            }
            else if (str[i] == '/' && i + 1 < str.length() && str[i + 1] == '*')
            {
                isMultiLineComment = true;
            }
            else if (str[i] == '/' && i + 1 < str.length() && str[i + 1] == '/')
            {
                isSingleLineComment = true;
            }
            else if (findChar(str[i], specialChars))
            {
                if (parsedString != "")
                {
                    cout << parsedString << endl;
                    parsedString = "";
                }
                else
                {
                    cout << str[i] << endl;
                    i++;
                }
            }
            else
            {
                parsedString += str[i];
                i++;
            }
        }
        if (parsedString != "")
        {
            cout << parsedString << endl;
        }
    }
    inFile.close();
}