#include <iostream>
#include <string>

using namespace std;

void print_decimal(int digits, string prefix) {
    if (digits == 0) {
        cout << prefix << endl;
    } else {
        for (int i = 0; i < 10; i++) {
            print_decimal(digits - 1, prefix + to_string(i));
        }
    }
}

int main(void) {
    print_decimal(2, "");
}