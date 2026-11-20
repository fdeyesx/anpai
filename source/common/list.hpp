#pragma once
#include <iostream>
#include "errors.hpp"

template <typename T>
class List;

template <typename T>
struct Node {
    T value; Node* next; Node* prev;

    Node(const T& val, Node<T>* n = nullptr, Node<T>* p = nullptr):value(val), next(n), prev(p) {}
};

template <typename T>
class Listiterator {
public:
    Node<T>* current;
    Listiterator() { current = nullptr; }
    ~Listiterator() {}
    T& operator*() const { return current->value; }
    bool operator==(const Listiterator& l2) { return current == l2.current; }
    bool operator!=(const Listiterator& l2) { return current != l2.current; }
    Listiterator<T>& operator=(const Listiterator& l2) { current = l2.current; return *this; }
    Listiterator<T>& operator++() { current = current->next; return *this; }

    friend class List<T>;
};

template <typename T>
class List {
protected:
    Node<T>* head; Node<T>* end; int count;

public:
    List() { head = nullptr; end = nullptr; count = 0; }
    List(Node<T>* begin): head(begin){
        int len = 0; Listiterator<T> it; it.current = begin;
        for (; it.current != nullptr; ++it)len++;
        end = it.current;
    }
    virtual ~List() { clear(); }

    List(const List& list) {
        if (list.head == nullptr) throw EmptyContainer();
        head = nullptr; end = nullptr;
        Node<T>* copy = list.head;
        while (copy != nullptr) {
            push_back(copy->value);
            copy = copy->next;
        }
    }

    friend class Listiterator<T>;

    const List& operator= (const List& list) {
        if (this == &list) throw SelfAssignment();

        List<T> tmp(list); clear();
        head = tmp.head;
        end = tmp.end;
        tmp.head = nullptr;
        tmp.end = nullptr;
        return *this;
    }

    void push_front(T value) {
        Node<T>* node = new Node<T>(value, head, nullptr);
        node->value = value;
        node->prev = nullptr;
        node->next = head;

        if (head != nullptr) head->prev = node;
        else end = node;
        head = node;
        count++;
    }

    void push_back(T value) {
        Node<T>* node = new Node<T>(value, head, nullptr);
        node->value = value;
        node->prev = end;
        node->next = nullptr;

        if (end != nullptr) end->next = node;
        else head = node;
        end = node;
        count++;
    }

    friend istream& operator>> (istream& in, List& list) {
        int n; in >> n;
        list.clear();
        for (int i = 0; i < n; i++) {
            T value; in >> value;
            list.push_back(value);
        }
        return in;
    }

    T pop_front() {
        if (head == nullptr) throw EmptyContainer();
        T value = head->value;
        Node<T>* tmp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else end = nullptr;
        delete tmp; count--;
        return value;
    }

    T pop_back() {
        if (end == nullptr) throw EmptyContainer();
        T value = end->value;
        Node<T>* tmp = end;
        end = end->prev;
        if (end != nullptr) end->next = nullptr;
        else head = nullptr;
        delete tmp; count--;
        return value;
    }

    friend ostream& operator<< (ostream& out, List& list) {
        Node<T>* cur = list.head;
        while (cur != nullptr) {
            out << cur->value << " -> ";
            cur = cur->next;
        }
        out << "NULL\n";
        return out;
    }

    void clear() {
        while (head != nullptr) {
            Node<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
        end = nullptr;
    }

    int getsize() { return count; }
    Listiterator<T> begin() { Listiterator<T> it; it.current = head; return it; }
    Listiterator<T> end_it() { Listiterator<T> it; it.current = end; return it; }

    bool remove(T value) {
        Node<T>* cur = head;
        bool result = false;
        while (cur != nullptr && result == false) {
            if (cur->value == value && result == false) {
                if (cur->prev) cur->prev->next = cur->next;
                else head = cur->next;

                if (cur->next) cur->next->prev = cur->prev;
                else end = cur->prev;

                delete cur;
                count--;
                result = true;
            }
            cur = cur->next;
        }
        return result;
    }

    void moveUp(T value) {
        Node<T>* cur = head;
        int flag = 0;
        while (cur != nullptr && flag == 0) {
            if (cur->value == value && flag == 0) {
                if (cur->prev != nullptr) {
                    T temp = cur->value;
                    cur->value = cur->prev->value;
                    cur->prev->value = temp;
                }
                flag = 1;
            }
            cur = cur->next;
        }
    }

    void moveDown(T value) {
        Node<T>* cur = head;
        int flag = 0;
        while (cur != nullptr && flag == 0) {
            if (cur->value == value && flag == 0) {
                if (cur->next != nullptr) {
                    T temp = cur->value;
                    cur->value = cur->next->value;
                    cur->next->value = temp;
                }
                flag = 1;
            }
            cur = cur->next;
        }
    }
};
