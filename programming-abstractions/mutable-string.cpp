#include <iostream>
#include <string>
using namespace std;

int main() {
    string s,t = "hello";
    s = t;
    t[0] = 'j';
    if (s == t) {
        cout << "s equals t" << endl;
    } else {
        cout << "s not equals t" << endl;
    }
}