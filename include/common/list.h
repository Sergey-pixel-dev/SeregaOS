#ifndef LIST_H
#define LIST_H
#include "stdint.h"

template <typename T>
class List
{
public:
    class Node
    {
    public:
        Node *next;
        Node *prev;
        T value;
    };

    Node *head = nullptr;
    Node *tail = nullptr;
    size_t sz = 0;

    void add(T value_to_add, Node *addr_to_alloc)
    {
        addr_to_alloc->value = value_to_add;
        addr_to_alloc->next = nullptr;

        if (head == nullptr)
        {
            addr_to_alloc->prev = nullptr;
            head = addr_to_alloc;
            tail = addr_to_alloc;
        }
        else
        {
            addr_to_alloc->prev = tail;
            tail->next = addr_to_alloc;
            tail = addr_to_alloc;
        }
        sz++;
    }

    Node *pop()
    {
        if (head == nullptr)
            return nullptr;
        Node *tmp = head;
        head = head->next;
        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;
        sz--;
        return tmp;
    }

    size_t size() { return sz; }
};

#endif