#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_vector(vector<string>*v) {
    for (int i = 0; i < v->size(); i++) {
        cout << v->at(i) << " ";
    }
    cout << endl;
}

void sublists_helper(vector<string>* v, vector<string>* chosen) {
    if (v->empty()) {
        print_vector(chosen);
    } else {
        // choose
        string first = v->at(0);
        v->erase(v->begin());
        // without first element
        sublists_helper(v, chosen);
        // with first element
        chosen->push_back(first);
        sublists_helper(v, chosen);
        chosen->pop_back();
        // un-choose
        v->insert(v->begin(), first);
    }
}

void sublists(vector<string>* v) {
    vector<string> chosen;
    sublists_helper(v, &chosen);
}

int main(void) {
    vector<string> v{"Jane", "Bob", "Matt", "Sara"};
    sublists(&v);
}