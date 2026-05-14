#pragma once

template <typename T> class IList {
public:
  virtual ~IList() = default;

  // Operacje dodawania
  virtual void push_front(T value) = 0;        // Początek
  virtual void push_back(T value) = 0;         // Koniec
  virtual void insert(T value, int index) = 0; // Losowe miejsce

  // Operacje usuwania
  virtual void pop_front() = 0;
  virtual void pop_back() = 0;
  virtual void remove(int index) = 0;

  // Operacje wyszukiwania
  virtual bool find(T value) = 0;

  // Metody pomocnicze przydatne w menu
  virtual void display() = 0;
  virtual void clear() = 0; // Do czyszczenia przed wczytaniem z pliku
  virtual int get_size() = 0;
};