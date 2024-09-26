#include <iostream>
#include <unordered_map>
using namespace std;
struct Student{
    string name;
    string email;
    string phone;
};

main(){

    unordered_map<int, Student> students;

    while(true){
        cout << "1- Insert" << endl;
        cout << "2- Remove" << endl;
        cout << "3- Print" << endl;
        cout << "4- Search" << endl;
        cout << "5- Exit" << endl;
        int option;
        cin >> option;

        if(option == 1){
            Student student;
            cout << "Enter Name: ";
            cin >> student.name;
            cout << "Enter Email: ";
            cin >> student.email;
            cout << "Enter Phone: ";
            cin >> student.phone;
            students[students.size()] = student;
        }else if(option == 2){
            int id;
            cout << "Enter ID: ";
            cin >> id;
            students.erase(id);
        }else if(option == 3){
            for(auto student : students){
                cout << student.first << " " << student.second.name << " " << student.second.email << " " << student.second.phone << endl;
            }
        }else if(option == 4){
            int id;
            cout << "Enter ID: ";
            cin >> id;
            if(students.find(id) != students.end()){
                cout << "Student found" << endl;
            }else{
                cout << "Student not found" << endl;
            }
        }else{
            break;
        }
    }
}