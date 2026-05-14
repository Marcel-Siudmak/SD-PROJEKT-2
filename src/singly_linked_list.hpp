#pragma once
#include "IList.hpp"

// Struktura pojedynczego węzła listy
template <typename T> struct singly_node {
  T data;
  singly_node *next;

  // Konstruktor pomocniczy
  singly_node(T val) : data(val), next(nullptr) {}
};

// Klasa listy jednokierunkowej dziedzicząca po IList
template <typename T> class singly_linked_list : public IList<T> {
private:
  singly_node<T> *head; // Wskaźnik na początek
  singly_node<T> *tail; // Wskaźnik na koniec
  int size;

public:
  singly_linked_list();
  ~singly_linked_list() override;

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