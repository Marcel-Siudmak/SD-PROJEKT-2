#pragma once

#include "IIList.hpp"
#include "pairing_heap.hpp"
#include <stdexcept>

// ============================================================
//  Wariant 1: Kolejka priorytetowa oparta na pairing_heap
//  Deleguje wszystkie operacje do wewnętrznej instancji heapa.
// ============================================================
template <typename T>
class pq_heap : public IIList<T> {
private:
    pairing_heap<T> heap_;

public:
    pq_heap() = default;
    ~pq_heap() override = default;

    // Rule of Five – heap_ zarządza własną pamięcią, więc
    // możemy polegać na domyślnym kopiowaniu/przenoszeniu.
    pq_heap(const pq_heap&) = delete;
    pq_heap& operator=(const pq_heap&) = delete;
    pq_heap(pq_heap&&) = default;
    pq_heap& operator=(pq_heap&&) = default;

    void insert(T value, int key) override;
<<<<<<< HEAD
    T    extract_max() override;
=======
    void extract_max() override;
>>>>>>> 6914417d7699cfa31f3ae194714712ea7ab00c99
    T    peek() override;
    void modify_key(T value, int new_key) override;
    int  return_size() override;
    void display() override;
    void clear() override;
};

// ============================================================
//  Węzeł wewnętrzny listy z priorytetem
// ============================================================
template <typename T>
struct pq_node {
    T   value;
    int key;
    pq_node<T>* next;

    pq_node(T v, int k) : value(v), key(k), next(nullptr) {}
};

// ============================================================
//  Wariant 2: Kolejka priorytetowa oparta na posortowanej
//  jednokierunkowej liście (malejąco według klucza).
//  peek()       → O(1)   – głowa to zawsze max
//  extract_max()→ O(1)   – usuń głowę
//  insert()     → O(n)   – wstaw w odpowiednie miejsce
//  modify_key() → O(n)   – znajdź, usuń i wstaw ponownie
// ============================================================
template <typename T>
class pq_list : public IIList<T> {
private:
    pq_node<T>* head_;
    int         size_;

    // Wewnętrzna funkcja wstawiania z zachowaniem porządku
    void insert_sorted(pq_node<T>* node);

public:
    pq_list();
    ~pq_list() override;

    pq_list(const pq_list&) = delete;
    pq_list& operator=(const pq_list&) = delete;
    pq_list(pq_list&&) = default;
    pq_list& operator=(pq_list&&) = default;

    void insert(T value, int key) override;
<<<<<<< HEAD
    T    extract_max() override;
=======
    void extract_max() override;
>>>>>>> 6914417d7699cfa31f3ae194714712ea7ab00c99
    T    peek() override;
    void modify_key(T value, int new_key) override;
    int  return_size() override;
    void display() override;
    void clear() override;
};
