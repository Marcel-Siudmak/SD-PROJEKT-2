#pragma once
#include "IIList.hpp"

template <typename T> struct Node {
  T _value;
  int _key;
  Node<T> *child;
  Node<T> *sibling;

  Node(T value, int key) : _value(value), _key(key), child(nullptr), sibling(nullptr) {}

  bool operator<(const Node<T> &other) const {
    return _key < other._key;
  }
};

template <typename T> class pairing_heap : public IIList<T> {
private:
  Node<T> *root;
  int _size;
  
  // Helper methods
  Node<T>* meld(Node<T>* heap1, Node<T>* heap2);
  Node<T>* twoPassMerge(Node<T>* firstSibling);
  void deleteTree(Node<T>* node);
  Node<T>* findNode(Node<T>* node, T value);
  bool disconnectNode(Node<T>*& node, T value);

public:
  pairing_heap();
  ~pairing_heap();

  void insert(T value, int key) override;
  T extract_max() override;
  T peek() override;
  void modify_key(T value, int new_key) override;
  int return_size() override;
  void display() override;
  void clear() override;

};
