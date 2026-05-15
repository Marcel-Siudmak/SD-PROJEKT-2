#include "priority_q.hpp"
#include <iostream>
#include <stdexcept>

// ============================================================
//  pq_heap – delegacja do pairing_heap<T>
// ============================================================

template <typename T> 
void pq_heap<T>::insert(T value, int key) {
  heap_.insert(value, key);
}

template <typename T> 
T pq_heap<T>::extract_max() {
  return heap_.extract_max();
}

template <typename T> 
T pq_heap<T>::peek() { return heap_.peek(); }

template <typename T> 
void pq_heap<T>::modify_key(T value, int new_key) {
  heap_.modify_key(value, new_key);
}

template <typename T> 
int pq_heap<T>::return_size() {
  return heap_.return_size();
}

template <typename T> 
void pq_heap<T>::display() { heap_.display(); }

template <typename T> 
void pq_heap<T>::clear() { heap_.clear(); }

// ============================================================
//  pq_list – posortowana jednokierunkowa lista (malejąco)
// ============================================================

template <typename T> 
pq_list<T>::pq_list() : head_(nullptr), size_(0) {}

template <typename T> 
pq_list<T>::~pq_list() { clear(); }

// Wstawia węzeł we właściwe miejsce listy posortowanej malejąco
template <typename T> 
void pq_list<T>::insert_sorted(pq_node<T> *node) {
  // Wstaw przed pierwszym węzłem o kluczu <= node->key
  if (!head_ || node->key >= head_->key) {
    node->next = head_;
    head_ = node;
    return;
  }

  pq_node<T> *cur = head_;
  while (cur->next && cur->next->key > node->key) {
    cur = cur->next;
  }
  node->next = cur->next;
  cur->next = node;
}

template <typename T> 
void pq_list<T>::insert(T value, int key) {
  pq_node<T> *node = new pq_node<T>(value, key);
  insert_sorted(node);
  ++size_;
}

template <typename T> 
T pq_list<T>::extract_max() {
  if (!head_) {
    throw std::runtime_error("extract_max: kolejka jest pusta");
  }
  pq_node<T> *old_head = head_;
  T val = old_head->value;
  head_ = head_->next;
  delete old_head;
  --size_;
  return val;
}

template <typename T> 
T pq_list<T>::peek() {
  if (!head_) {
    throw std::runtime_error("peek: kolejka jest pusta");
  }
  return head_->value;
}

template <typename T> 
void pq_list<T>::modify_key(T value, int new_key) {
  // Znajdź węzeł z podaną wartością, usuń go z listy,
  // zaktualizuj klucz i wstaw z powrotem w posortowanej pozycji.
  pq_node<T> *cur = head_;
  pq_node<T> *prev = nullptr;

  while (cur) {
    if (cur->value == value) {
      // Odepnij węzeł
      if (prev) {
        prev->next = cur->next;
      } else {
        head_ = cur->next;
      }
      cur->next = nullptr;
      cur->key = new_key;
      insert_sorted(cur);
      return;
    }
    prev = cur;
    cur = cur->next;
  }

  throw std::runtime_error("modify_key: wartość nie została znaleziona");
}

template <typename T> 
int pq_list<T>::return_size() { return size_; }

template <typename T> 
void pq_list<T>::display() {
  if (!head_) {
    std::cout << "Kolejka jest pusta.\n";
    return;
  }
  std::cout << "Kolejka priorytetowa (wartość: klucz):\n";
  pq_node<T> *cur = head_;
  while (cur) {
    std::cout << "  " << cur->value << " : " << cur->key << "\n";
    cur = cur->next;
  }
}

template <typename T> 
void pq_list<T>::clear() {
  while (head_) {
    pq_node<T> *tmp = head_;
    head_ = head_->next;
    delete tmp;
  }
  size_ = 0;
}

// ============================================================
//  Jawna instancja szablonów
// ============================================================
template class pq_heap<int>;
template class pq_list<int>;
