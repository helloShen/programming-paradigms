#include <iostream>
#include <string>

using namespace std;

void print_binary(int digit, string prefix) {
    if (digit == 0) { // base case
        cout << prefix << endl;
    } else {
        print_binary(digit - 1, prefix + "0");
        print_binary(digit - 1, prefix + "1");
    } 
}

int main(void) {
    print_binary(4, "");
}