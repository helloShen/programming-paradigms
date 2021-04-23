#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#include <string>
using namespace std;

class Employee {
    string name;
    int salary;
public:
    Employee(string name, int salary);
    virtual string get_name(void) const; // const means: this function doesn't change fields in class Emplyee
    virtual int get_salary(void) const;
    virtual void speak(void) const = 0; // pure virtual member function, require sub-class to implement
};

#endif // _EMPLOYEE_H_