#include <iostream>
#include <string>
#include "lawyer.h"
using namespace std;

Lawyer::Lawyer(string name, int salary, string law_school) : Employee(name, salary) {
    this->law_school = law_school;
}

string Lawyer::get_law_school(void) const {
    return law_school;
}

int Lawyer::get_salary(void) const {
    return Employee::get_salary() * 2;
}

void Lawyer::speak(void) const {
    cout << "Hello World!" << endl;
}

int main(void) {
    Lawyer* tom = new Lawyer("Tom", 1000, "Harvard");
    cout << tom->get_law_school() << endl;
    cout << tom->get_salary() << endl;
    tom->speak();
}