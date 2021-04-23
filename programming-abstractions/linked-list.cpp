#include <iostream>
#include "linked-list.h"

int LinkedList::size(void) {
    int n = 0;
    ListNode* curr = front;
    while (curr != nullptr) {
        n++;
        curr = curr->next;
    }
    return n;
}
void LinkedList::add(int value) {
    ListNode *new_node = new ListNode(value);
    if (front == nullptr) {
        front = new_node;
    } else {
        ListNode *curr = front;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}
int LinkedList::get(int index) {
    if (front == nullptr) {
        return 0;
    } else {
        ListNode *curr = front;
        while (index - 1 > 0) {
            if (curr == nullptr) return 0;
            curr = curr->next;
            index--;
        }
        if (curr == nullptr) {
            return 0;
        } else {
            return curr->data;
        }
    }
    
}
void LinkedList::add_first(int value) {
    ListNode *new_node = new ListNode(value);
    new_node->next = front;
    front = new_node;
}
void LinkedList::remove_first(void) {
    if (front != nullptr) {
        ListNode *trash = front;
        front = front->next;
        delete trash;
    }
}
void LinkedList::remove(int index) {
    ListNode* curr = front;
    while (index - 2 > 0) {
        if (curr == nullptr) return;
        curr = curr->next;
        index--;
    }
    if (curr != nullptr && curr->next != nullptr) {
        ListNode* trash = curr->next;
        curr->next = curr->next->next;
        delete trash;
    }
}

int main(void) {
    // test code
}