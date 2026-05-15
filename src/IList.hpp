#pragma once

// Interfejs listy jednokierunkowej (z projektu SD-PROJEKT-1).
// Zachowany dla kompatybilności z singly_linked_list.hpp/.cpp.
template <typename T>
class IList {
public:
    virtual ~IList() = default;

    virtual void push_front(T value) = 0;
    virtual void push_back(T value)  = 0;
    virtual void insert(T value, int index) = 0;

    virtual void pop_front() = 0;
    virtual void pop_back()  = 0;
    virtual void remove(int index) = 0;

    virtual bool find(T value) = 0;

    virtual void display()  = 0;
    virtual void clear()    = 0;
    virtual int  get_size() = 0;
};
