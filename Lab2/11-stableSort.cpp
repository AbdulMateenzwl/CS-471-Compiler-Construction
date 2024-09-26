#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct Employee
{
    string name;
    string department;
    string hire_date;
};

bool compareByDepartment(const Employee &a, const Employee &b)
{
    return a.department < b.department;
}

int main()
{
    vector<Employee> employees = {
        {"saqib", "HR", "2023-01-15"},
        {"khadim", "Engineering", "2023-03-10"},
        {"mateen", "HR", "2024-05-23"},
        {"nasir", "Engineering", "2024-07-29"},
        {"Ali", "Marketing", "2024-02-14"}};

    stable_sort(employees.begin(), employees.end(), compareByDepartment);

    for (const auto &employee : employees)
    {
        cout << employee.name << " - " << employee.department << " - Hire Date: " << employee.hire_date << endl;
    }

    return 0;
}
