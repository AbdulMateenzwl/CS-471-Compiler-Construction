#include <iostream>
#include <unordered_map>
using namespace std;
main()
{
    string str = " Create a word frequency counter using an unordered map that counts how often each word appears in a given text. Compare the performance of this solution with an ordered map in terms of insertion and lookup time. ";

    unordered_map<string, int> wordCounter;

    // cout << "Enter Text to analyze" << endl;
    // cin>>str;

    string word = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ' && word != "")
        {
            wordCounter[word]++;
            word = "";
        }
        else
        {
            word += str[i];
        }
    }

    cout<<"Find Word: ";
    cin>>word;
    cout<<"Frequency of word "<<word<<" is "<<wordCounter[word]<<endl;
}