#ifndef STACK_H
#define STACK_H

#include <stdexcept>

// stack.h will define a simple stack class template.
// It will support basic operations like push, pop, top, size, and clear.
// The stack will be implemented using a linked list structure.
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* topNode;
    size_t count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }

    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Stack underflow");
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        count--;
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return topNode->data;
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Stack is empty");
        return topNode->data;
    }

    bool empty() const { return topNode == nullptr; }
    size_t size() const { return count; }

    void clear() {
        while (!empty()) {
            pop();
        }
    }
};

#endif
