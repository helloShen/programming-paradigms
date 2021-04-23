#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int calculate(int a, int b, int op) {
    switch(op) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return a/b;
        default: return 0;
    }
}

int evaluate_helper(const string& exp, int& index) {
    if (isdigit(exp[index])) { // base case
        return exp[index++] - '0';
    } else {
        index++;
        int left = evaluate_helper(exp, index);
        int op = exp[index++];
        int right = evaluate_helper(exp, index);
        index++;
        return calculate(left, right, op);
    }
}

/**
 * (2*((3+4)*5)=70
 */
int evaluate(const string& exp) {
    int index = 0;
    return (evaluate_helper(exp, index));
}

int main() {
    cout << evaluate("((1+(2*3))+4)") << endl; // 11
    cout << evaluate("((2*(3+4))*5)") << endl; // 70
}