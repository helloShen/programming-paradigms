#include <string>
#include "employee.h"
using namespace std;

Employee::Employee(string name, int salary) {
    this->name = name;
    this->salary = salary;
}

string Employee::get_name(void) const {
    return name;
}

int Employee::get_salary(void) const {
    return salary;
}