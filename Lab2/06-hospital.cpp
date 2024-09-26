#include <iostream>
#include <queue>
using namespace std;

int main()
{
    priority_queue<pair<int, string>> pq;

    while (true)
    {
        cout << "1- Insert Patient" << endl;
        cout << "2- Treat Patient" << endl;
        cout << "3- Exit"<<endl;
        int option;
        cin>>option;
        if(option == 1){
            cout<<"Enter Person Name:";
            string name;
            cin>>name;
            cout<<"Rate Emergency Level 1-10:";
            int emergency;
            cin>>emergency;
            pq.push(make_pair(emergency,name));
        }
        else if(option == 2){
            if(pq.size() == 0){
                cout<<"Queue is Empty"<<endl;
            }
            else{
                cout<<"Patient Treated: "<<pq.top().second<<endl;
                pq.pop();
            }
        }
        else{
            break;
        }
    }
}