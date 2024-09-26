#include <iostream>
#include <stack>
using namespace std;
bool isValid(string s) {
        stack<char> stk;
        for (char c : s) {
            if(c=='(' || c == '[' || c == '{'){
                stk.push(c);
            }
            else{
                if(stk.empty())
                    return false;
                if(abs(stk.top()-c) <= 2)
                    stk.pop();
                else
                    return false;
            }
        }
        return stk.empty();
    }
main()
{
    string str = "{[({)]}";
    cout << isValid(str) << endl;
}