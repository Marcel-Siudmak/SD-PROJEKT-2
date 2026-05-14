#include "array_list.hpp"
#include <iostream>
template <typename T>

array_list<T>::~array_list(){
    delete[] _arr;
}
template <typename T>
array_list<T>::array_list() : _arr(nullptr), _size(0), _capacity(0) {}; //pytanie czy chcemy na pusto dawac 0 size czy zakladamy ze bedzie ktos przynajmniej 1 element zaraz wsadzal
template <typename T>
array_list<T>::array_list(int cap) : _capacity(cap), _arr(new T[_capacity]), _size(0) {};
template <typename T>
array_list<T>::array_list(const array_list& other){
            _capacity = other._capacity;
            _size = other._size;
            _arr = new T[_capacity];
            for(int i = 0; i < _size; i++){
                _arr[i] = other._arr[i];
            }
        };
template <typename T>
array_list<T>::array_list(const array_list& other, int cap):_capacity(cap), _arr(new T[_capacity]), _size(other._size > cap? cap : other._size){
            for(int i = 0; i < _size; i++){
                _arr[i] = other._arr[i];
            }
        };
template <typename T>
void array_list<T>::resize(int new_capacity){
            T* new_arr = new T[new_capacity];
            for(int i = 0; i < _size; i++){
                new_arr[i] = _arr[i];
            }
            delete[] _arr;
            _arr = new_arr;
            _capacity = new_capacity;
        }
template <typename T>
array_list<T>& array_list<T>::operator=(const array_list& other){
            if(this == &other) return *this;
            delete[] _arr;
            _capacity = other._capacity;
            _size = other._size;
            _arr = new T[_capacity];
            for(int i = 0; i < _size; i++){
                _arr[i] = other._arr[i];
            }
            return *this;
        };
template <typename T>
void array_list<T>::display(){

    for(int i = 0; i < _size; i++){
        std::cout << "[" << _arr[i] << "] ";
        (i+1) % 10 == 0 ? std::cout << std::endl : std::cout << "";
    }
    std::cout << std::endl;
}
template <typename T>
void array_list<T>::clear(){
    delete[] _arr;
    _arr = nullptr;
    _size = 0;
    _capacity = 0;
}
template <typename T>
void array_list<T>::push_back(T value){
    if(_size == _capacity) resize(_capacity == 0 ? 1 : _capacity * 2);
    _arr[_size++] = value;
}
template <typename T>
void array_list<T>::push_front(T value){
    if(_size == _capacity) resize(_capacity == 0 ? 1 : _capacity * 2);
    for(int i = _size; i > 0; i--){
        _arr[i] = _arr[i-1];
    }
    _arr[0] = value;
    _size++;
}
template <typename T>
void array_list<T>::insert(T value, int index){
    if(index < 0 || index > _size) return;
    if(_size == _capacity) resize(_capacity == 0 ? 1 : _capacity * 2);
    for(int i = _size; i > index; i--){
        _arr[i] = _arr[i-1];
    }
    _arr[index] = value;
    _size++;
}
template <typename T>
void array_list<T>::pop_back(){
    if(_size == 0) return;
    _size--;
    if(_size < _capacity / 4) resize(_capacity / 2);
}
template <typename T>
void array_list<T>::pop_front(){
    if(_size == 0) return;
    for(int i = 0; i < _size - 1; i++){
        _arr[i] = _arr[i+1];
    }
    _size--;
    if(_size < _capacity / 4) resize(_capacity / 2);
}
template <typename T>
void array_list<T>::remove(int index){
    if(index < 0 || index >= _size) return;
    for(int i = index; i < _size - 1; i++){
        _arr[i] = _arr[i+1];
    }
    _size--;
    if(_size < _capacity / 4) resize(_capacity / 2);
}
template <typename T>
bool array_list<T>::find(T value){
    for(int i = 0; i < _size; i++){
        if(_arr[i] == value) return true;
    }
    return false;
}
template <typename T>
int array_list<T>::get_size(){
    return _size;
}

// Explicit template instantiation
template class array_list<int>;
