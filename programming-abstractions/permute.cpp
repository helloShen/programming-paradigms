#include <iostream>
#include <string>

using namespace std;

void permute(string s, string prefix) {
    if (s.length() == 0) {
        cout << prefix << endl;
    } else {
        for (int i = 0; i < s.length(); i++) {
            char c = s[i];
            string remain = s.substr(0, i) + s.substr(i+1);
            permute(remain, prefix + c);
        }
    }
}

int main(void) {
    permute("MARTY", "");
}