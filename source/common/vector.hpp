#pragma once
#include <iostream>
#include "errors.hpp"

template <typename T>
class Array;

template <typename T>
class Arrayiterator {
public:
    T* current; int size;
    Arrayiterator() { current = nullptr; }
    ~Arrayiterator() {}
    T& operator*() const { return *current; }
    bool operator==(const Arrayiterator& l2) { return current == l2.current; }
    bool operator!=(const Arrayiterator& l2) { return current != l2.current; }
    Arrayiterator& operator=(const Arrayiterator& l2) { current = l2.current; size = l2.size; return *this; }
    Arrayiterator& operator++() { current = current + 1; return *this; }

    Arrayiterator operator+(int n) { Arrayiterator it; it.current = current + n; return it; }
    Arrayiterator operator-(int n) { Arrayiterator it; it.current = current - n; return it; }

    int operator-(const Arrayiterator& l2) { return current - l2.current; }

    bool operator<(const Arrayiterator& l2) { return current < l2.current; }
    bool operator<=(const Arrayiterator& l2) { return current <= l2.current; }
    bool operator>(const Arrayiterator& l2) { return current > l2.current; }
    bool operator>=(const Arrayiterator& l2) { return current >= l2.current; }

    T& operator[](int n) { return current[n]; }
    T* operator->() const { return current; }
    friend class Array<T>;
};

template <typename T>
class Array {
protected:
    int size = 0; int capacity = 10; T* data;

    void resize() {
        capacity *= 2;
        T* tmp = new T[capacity];
        for (int i = 0; i < size; i++) tmp[i] = data[i];
        delete[] data; data = tmp;
    }

    void merge(int left, int right, int mid) {
        int n1 = mid - left + 1; int n2 = right - mid;
        T* t1 = new int[n1]; T* t2 = new int[n2];

        Arrayiterator<T> array_l; array_l.current = data + left;
        Arrayiterator<T> array_r; array_r.current = data + mid + 1;

        Arrayiterator<T> t1_i; t1_i.current = t1;
        Arrayiterator<T> t2_i; t2_i.current = t2;

        for (int i = 0; i < n1; i++) t1[i] = array_l[i];
        for (int i = 0; i < n2; i++) t2[i] = array_r[i];

        t1_i.current = t1; t2_i.current = t2;
        Arrayiterator<T> t1_end = t1_i + n1; Arrayiterator<T> t2_end = t2_i + n2;
        Arrayiterator<T> res_it; res_it.current = data + left;

        while (t1_i < t1_end && t2_i < t2_end) {
            if (*t1_i <= *t2_i) {
                *res_it = *t1_i;
                ++t1_i;
            }
            else if (*t1_i > *t2_i) {
                *res_it = *t2_i;
                ++t2_i;
            }
            ++res_it;
        }

        while (t1_i < t1_end) {
            *res_it = *t1_i;
            ++t1_i; ++res_it;
        }
        while (t2_i < t2_end) {
            *res_it = *t2_i;
            ++t2_i; ++res_it;
        }

        delete[]t1; delete[]t2;
    }

    void merge_sort(int left, int right) {
        if (left >= right) return;
        int middle = left + (right - left) / 2;

        merge_sort(left, middle); merge_sort(middle + 1, right); merge(left, right, middle);
    }

public:
    using iterator = Arrayiterator<T>;
    Array() { data = new T[capacity]; }
    virtual ~Array() { delete[] data; }

    Arrayiterator<T> begin() {
        Arrayiterator<T> it; 
        it.current = data; 
        it.size = size; 
        return it; 
    }
    Arrayiterator<T> end() { 
        Arrayiterator<T> it; 
        it.current = data + size;
        it.size = size;
        return it; 
    }

    Array(const Array<T>& arr) {
        size = arr.size;
        capacity = arr.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; i++) data[i] = arr.data[i];
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) throw IndexOutOfRange();;
        return data[index];
    }

    Array& operator= (const Array<T>& arr) {
        if (this == &arr) throw SelfAssignment();

        int new_capacity = (arr.capacity > arr.size) ? arr.capacity : arr.size;
        T* newdata = new T[new_capacity];
        for (int i = 0; i < arr.size; i++) newdata[i] = arr.data[i];
        delete[] data;

        data = newdata;
        size = arr.size;
        capacity = arr.capacity;

        return *this;
    }

    T pop_back() {
        return size == 0 ? throw EmptyContainer() : data[--size];
    }

    T pop_front() {
        if (size == 0) throw EmptyContainer();
        T val = data[0];
        for (int i = 0; i < size; i++) data[i] = data[i + 1];
        size--;
        return val;
    }

    virtual void push_back(T value) {
        if (size == capacity) resize();
        data[size++] = value;
    }

    virtual void push_front(T value) {
        if (size == capacity) resize();
        for (int i = size; i > 0; --i) data[i] = data[i - 1];
        data[0] = value; size++;
    }

    void clear() {
        size = 0;
    }

    int getsize() const { return size; }

    void sort() {
        if (size > 1) merge_sort(0, size - 1);
    }
};;

template <typename T>
ostream& operator<< (ostream& out, const Array<T>& arr) {
    for (int i = 0; i < arr.size; ++i) {
        out << arr.data[i];
        if (i + 1 < arr.size) out << " ";
    }
    return out;
}

template <typename T>
istream& operator>> (istream& in, Array<T>& arr) {
    int n; in >> n;
    if (n < 0) throw NegativeSize();
    for (int i = 0; i < n; i++) {
        T value; in >> value;
        arr.push_back(value);
    }
    return in;
}
