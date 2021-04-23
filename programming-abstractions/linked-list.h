#ifndef _LINKED_LIST_
#define _LINKED_LIST_

class ListNode {
public:
    int data;
    ListNode *next;
    ListNode(int value) {
        data = value;
        next = nullptr;
    }
};

class LinkedList {
    ListNode* front;
public:
    int size(void);
    void add(int value);
    int get(int index);
    void add_first(int value);
    void remove_first(void);
    void remove(int index);
};
#endif // _LINKED_LIST_