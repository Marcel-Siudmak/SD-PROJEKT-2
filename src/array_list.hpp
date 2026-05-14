#pragma once
#include "IList.hpp"

template <class T> 
class array_list : public IList<T> {
    private:
        T* _arr;
        int _size;
        int _capacity;
        void resize(int new_capacity);
    public:
        array_list();
        array_list(int cap);
        array_list(const array_list& other);
        array_list(const array_list& other, int cap);
        ~array_list();

        array_list& operator=(const array_list& other);
        
        void push_front(T value) override;
        void push_back(T value) override;
        void insert(T value, int index) override;

        void pop_front() override;
        void pop_back() override;
        void remove(int index) override;

        bool find(T value) override;

        void display() override;
        void clear() override;
        int get_size() override;
};
