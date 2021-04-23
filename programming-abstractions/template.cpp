#include <iostream>
#include <string>
using namespace std;

template<typename T> T max_value(T a, T b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}

template<typename T> class Pair {
    T x;
    T y;
public:
    Pair(T x, T y) {
        this->x = x;
        this->y = y;
    }
    void print(void) const {
        cout << "[" << x << "," << y << "]" << endl;
    }
};

int main(void) {
    int i = max(17, 4);
    cout << "max(17, 4) = " << max_value(17, 4) << endl;
    Pair<int> p* = new Pair<int>(2,3);
    p->print();
}

