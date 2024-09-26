#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct Student
{
    string name;
    int grade;
};

bool customComparator(const Student &a, const Student &b)
{
    if (a.grade != b.grade)
        return a.grade > b.grade;
    return a.name < b.name;
}

int main()
{
    vector<Student> students = {
        {"Saqib", 85},
        {"Khadim", 92},
        {"Mateen", 85},
        {"Ali", 92},
        {"Nasir", 75}};

    sort(students.begin(), students.end(), customComparator);

    for (const auto &student : students)
    {
        cout << student.name << ": " << student.grade << endl;
    }

    return 0;
}
