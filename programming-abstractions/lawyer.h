#ifndef _LAWYER_H_
#define _LAWYER_H_
#include <string>
#include "employee.h"

class Lawyer : public Employee {
    string law_school;
public:
    Lawyer(string name, int years, string law_school);
    virtual string get_law_school(void) const;
    int get_salary(void) const;
    virtual void speak(void) const;
};

#endif // _LAWYER_H_