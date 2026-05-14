#include "singly_linked_list.hpp"
#include <iostream>

template <typename T> singly_linked_list<T>::singly_linked_list() {
  head = nullptr;
  tail = nullptr;
  size = 0;
}

template <typename T> singly_linked_list<T>::~singly_linked_list() { clear(); }

template <typename T> void singly_linked_list<T>::push_front(T value) {
  singly_node<T> *new_node = new singly_node<T>(value);
  if (size == 0) {
    head = tail = new_node;
  } else {
    new_node->next = head;
    head = new_node;
  }
  size++;
}

template <typename T> void singly_linked_list<T>::push_back(T value) {
  singly_node<T> *new_node = new singly_node<T>(value);
  if (size == 0) {
    head = tail = new_node;
  } else {
    tail->next = new_node;
    tail = new_node;
  }
  size++;
}

template <typename T> void singly_linked_list<T>::insert(T value, int index) {
  if (index < 0 || index > size)
    return;

  if (index == 0) {
    push_front(value);
  } else if (index == size) {
    push_back(value);
  } else {
    singly_node<T> *new_node = new singly_node<T>(value);
    singly_node<T> *temp = head;
    // Przechodzimy do elementu przed miejscem wstawienia
    for (int i = 0; i < index - 1; i++)
      temp = temp->next;

    new_node->next = temp->next;
    temp->next = new_node;
    size++;
  }
}

template <typename T> void singly_linked_list<T>::pop_front() {
  if (size == 0)
    return;

  singly_node<T> *temp = head;
  head = head->next;
  delete temp; // Ręczne zwalnianie pamięci

  size--;
  if (size == 0) {
    tail = nullptr; // Jeśli usunęliśmy ostatni element, tail też musi być puste
  }
}

template <typename T> void singly_linked_list<T>::pop_back() {
  if (size == 0)
    return;

  if (size == 1) {
    delete head;
    head = tail = nullptr;
  } else {
    singly_node<T> *temp = head;
    // Szukamy przedostatniego elementu
    while (temp->next != tail) {
      temp = temp->next;
    }
    delete tail;
    tail = temp;
    tail->next = nullptr;
  }
  size--;
}

template <typename T> void singly_linked_list<T>::remove(int index) {
  if (index < 0 || index >= size)
    return;

  if (index == 0) {
    pop_front();
  } else if (index == size - 1) {
    pop_back();
  } else {
    singly_node<T> *temp = head;
    for (int i = 0; i < index - 1; i++)
      temp = temp->next;

    singly_node<T> *node_to_delete = temp->next;
    temp->next = node_to_delete->next;
    delete node_to_delete;
    size--;
  }
}

template <typename T> bool singly_linked_list<T>::find(T value) {
  singly_node<T> *temp = head;
  while (temp != nullptr) {
    if (temp->data == value) {
      return true; // Znaleziono element
    }
    temp = temp->next;
  }
  return false; // Nie znaleziono
}

template <typename T> void singly_linked_list<T>::display() {
  singly_node<T> *temp = head;
  while (temp != nullptr) {
    std::cout << temp->data << " -> ";
    temp = temp->next;
  }
  std::cout << "NULL\n";
}

template <typename T> void singly_linked_list<T>::clear() {
  while (size > 0) {
    pop_front(); // Zwalnia pamięć węzeł po węźle
  }
}

template <typename T> int singly_linked_list<T>::get_size() { return size; }

template class singly_linked_list<int>;