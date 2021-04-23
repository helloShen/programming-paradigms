#include <iostream>
#include <string>

using namespace std;

class Board {
    int size = 8;
    int board[8][2];
    int num = 0;
    
    public:
        /* return the size of board */
        int get_size(void) {
            return size;
        }
        /* check if this position is safe to put new queen */
        bool is_safe(int row, int column) {
            for (int i = 0; i < num; i++) {
                if (board[i][0] == row) return false;
                if (board[i][1] == column) return false;
                if (board[i][0] - row == board[i][1] - column) return false;
                if (board[i][0] - row == column - board[i][1]) return false;
            }
            return true;
        }
        /* place queen here */
        void place(int row, int column) {
            if (num == 64) return;
            board[num][0] = row;
            board[num][1] = column;
            num++;
        }
        /* remove queen from here */
        void remove(int row, int column) {
            if (num == 0) return;
            num--;
        }
        /* print all 8 queens on the board */ 
        void print(void) {
            for (int i = 0; i < num; i++) {
                cout << "[" << board[i][0] << "," << board[i][1] << "]";
            }
            cout << endl;
        }

};

void eight_queens(Board* b, const int row) {
    if (row == b->get_size()) { // base case
        b->print();
    } else {
        for (int i = 0; i < b->get_size(); i++) {
            if (b->is_safe(row, i)) {
                b->place(row, i);
                eight_queens(b, row + 1);
                b->remove(row, i);
            }
        }
    }
}

int main(void) {
    Board b;
    eight_queens(&b, 0);
}

