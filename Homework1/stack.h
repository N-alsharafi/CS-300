//
// Created by nuh alsharafi on 7/24/24.
//

#ifndef STACK_H_
#define STACK_H_

using namespace std;

template <class Object>
class Stack
{
public:
    Stack();
    Stack(const Stack & rhs);
    ~Stack();

    bool isEmpty() const;
    bool isFull() const;
    void makeEmpty();

    void pop();
    void push(const Object & x);
    Object topAndPop();
    const Object & top() const;

    const Stack & operator=(const Stack & rhs);

private:
    struct ListNode
    {
        Object   element;
        ListNode *next;

        ListNode()
        {
            next = nullptr;
            //default constructor sets pointer to null
        }

        ListNode( const Object & theElement, ListNode * n = nullptr )
            : element( theElement ), next( n ) { }
    };

    ListNode *topOfStack;  // list itself is the stack
};

#include "stack.cpp"

#endif //STACK_H_
