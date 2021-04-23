#include <iostream>
#include <string>

using namespace std;

void travel_helper(const int to_x, const int to_y, const int from_x, const int from_y, string path) {
    if (to_x == from_x && to_y == from_y) {
        cout << path << endl;
    } else {
        // go east
        if (to_x > from_x) {
            travel_helper(to_x, to_y, from_x + 1, from_y, path + " E");
        }
        // go south
        if (to_y > from_y) {
            travel_helper(to_x, to_y, from_x, from_y + 1, path + " S");
        }
        // go south-east
        if (to_x > from_x && to_y > from_y) {
            travel_helper(to_x, to_y, from_x + 1, from_y + 1, path + " SE");
        }
    }
}

void travel(const int x, const int y) {
    travel_helper(x, y, 0, 0, "");
}

int main(void) {
    travel(2,1);
}