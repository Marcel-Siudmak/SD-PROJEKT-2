#include "doubly_linked_list.hpp"
#include <iostream>

template <typename T> doubly_linked_list<T>::doubly_linked_list() {
  head = nullptr;
  tail = nullptr;
  size = 0;
}

template <typename T> doubly_linked_list<T>::~doubly_linked_list() { clear(); }

template <typename T> void doubly_linked_list<T>::push_front(T value) {
  doubly_node<T> *new_node = new doubly_node<T>(value);
  if (size == 0) {
    head = tail = new_node;
  } else {
    new_node->next = head;
    head->prev = new_node;
    head = new_node;
  }
  size++;
}

template <typename T> void doubly_linked_list<T>::push_back(T value) {
  doubly_node<T> *new_node = new doubly_node<T>(value);
  if (size == 0) {
    head = tail = new_node;
  } else {
    new_node->prev = tail;
    tail->next = new_node;
    tail = new_node;
  }
  size++;
}

template <typename T> void doubly_linked_list<T>::insert(T value, int index) {
  if (index < 0 || index > size)
    return;

  if (index == 0) {
    push_front(value);
  } else if (index == size) {
    push_back(value);
  } else {
    doubly_node<T> *new_node = new doubly_node<T>(value);
    if (index < size / 2) {
      doubly_node<T> *temp = head;

      for (int i = 0; i < index - 1; i++)
        temp = temp->next;

      new_node->next = temp->next;
      new_node->prev = temp;
      temp->next->prev = new_node;
      temp->next = new_node;
    } else {
      doubly_node<T> *temp = tail;

      for (int i = size - 1; i > index; i--)
        temp = temp->prev;

      new_node->prev = temp->prev;
      new_node->next = temp;
      temp->prev->next = new_node;
      temp->prev = new_node;
    }
    size++;
  }
}

template <typename T> void doubly_linked_list<T>::pop_front() {
  if (size == 0)
    return;

  doubly_node<T> *temp = head;
  head = head->next;
  
  if (head != nullptr) {
    head->prev = nullptr;
  }
  
  delete temp;
  size--;

  if (size == 0)
    tail = nullptr;
}

template <typename T> void doubly_linked_list<T>::pop_back() {
  if (size == 0)
    return;

  doubly_node<T> *temp = tail;
  tail = tail->prev;
  
  if (tail != nullptr) {
    tail->next = nullptr;
  }
  
  delete temp;
  size--;

  if (size == 0)
    head = nullptr;
}

template <typename T> void doubly_linked_list<T>::remove(int index) {
  if (index < 0 || index >= size)
    return;

  if (index == 0) {
    pop_front();
  } else if (index == size - 1) {
    pop_back();
  } else {

    if (index < size / 2) {
      doubly_node<T> *temp = head;
      for (int i = 0; i < index - 1; i++)
        temp = temp->next;

      doubly_node<T> *to_delete = temp->next;
      temp->next = to_delete->next;
      to_delete->next->prev = temp;
      delete to_delete;
    } else {
      doubly_node<T> *temp = tail;
      for (int i = size - 1; i > index + 1; i--)
        temp = temp->prev;

      doubly_node<T> *to_delete = temp->prev;
      temp->prev = to_delete->prev;
      to_delete->prev->next = temp;
      delete to_delete;
    }
    size--;
  }
}

template <typename T> bool doubly_linked_list<T>::find(T value) {
  doubly_node<T> *temp = head;
  while (temp != nullptr) {
    if (temp->data == value)
      return true;
    temp = temp->next;
  }
  return false;
}

template <typename T> void doubly_linked_list<T>::display() {
  doubly_node<T> *temp = head;
  while (temp != nullptr) {
    std::cout << temp->data << " ";
    temp = temp->next;
  }
  std::cout << std::endl;
}

template <typename T> void doubly_linked_list<T>::clear() {
  while (size > 0) {
    pop_front();
  }
}

template <typename T> int doubly_linked_list<T>::get_size() { return size; }

template class doubly_linked_list<int>;