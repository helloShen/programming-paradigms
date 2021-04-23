#include <iostream>
#include <string>
#include <vector>

using namespace std;

void dice_roll_helper(int dice, vector<string>* chosen) {
    if(dice == 0) {
        for (int i = 0; i < chosen->size(); i++) {
            cout << chosen->at(i);
        }
        cout << endl;
    } else {
        for (int i = 1; i <=6; i++) {
            chosen->push_back(to_string(i));
            dice_roll_helper(dice - 1, chosen);
            chosen->pop_back();
        }
    }
}

void dice_roll(int dice) {
    vector<string> v;
    dice_roll_helper(dice, &v);
}

int sum_vector(const vector<string>* chosen) {
    int sum = 0;
    for (int i = 0; i < chosen->size(); i++) {
        sum += stoi(chosen->at(i));
    }
    return sum;
}

void dice_sum_helper(int dice, int sum, int sum_sofar, vector<string>* chosen, int* calls) {
    (*calls)++;
    if(dice == 0) {
        if (sum_sofar == sum) {
            for (int i = 0; i < chosen->size(); i++) {
                cout << chosen->at(i);
            }
            cout << endl;
        }
    } else {
        for (int i = 1; i <=6; i++) {
            int min = sum_sofar + i + (dice - 1) * 1;
            int max = sum_sofar + i + (dice - 1) * 6;
            if (min <= sum && max >= sum) { // pruning tree
                // back tracking
                chosen->push_back(to_string(i));
                dice_sum_helper(dice - 1, sum, sum_sofar + i, chosen, calls);
                chosen->pop_back();
            }
        }
    }
}

void dice_sum(int dice, int sum) {
    vector<string> v;
    int calls = 0;
    dice_sum_helper(dice, sum, 0, &v, &calls);
    cout << "calls = " << calls << endl;
}

int main(void) {
    // dice_roll(3);
    dice_sum(2,7);
}