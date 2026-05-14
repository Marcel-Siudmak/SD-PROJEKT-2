#pragma once
#include "IList.hpp"

template <typename T> struct doubly_node {
  T data;
  doubly_node<T> *next;
  doubly_node<T> *prev;

  doubly_node(T data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename T> class doubly_linked_list : public IList<T> {
private:
  doubly_node<T> *head;
  doubly_node<T> *tail;
  int size;

public:
  doubly_linked_list();
  ~doubly_linked_list();

  // Operacje dodawania
  void push_front(T value) override;
  void push_back(T value) override;
  void insert(T value, int index) override;

  // Operacje usuwania
  void pop_front() override;
  void pop_back() override;
  void remove(int index) override;

  // Wyszukiwanie
  bool find(T value) override;

  // Metody pomocnicze
  void display() override;
  void clear() override;
  int get_size() override;
};
