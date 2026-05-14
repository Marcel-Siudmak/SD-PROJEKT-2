#pragma once

template <typename T> class IIList {
public:
  virtual ~IIList() = default;


    virtual void insert(T value, int key) = 0;
    virtual void extract_max() = 0;
    virtual T peek() = 0;
    virtual void modify_key(T value, int new_key) = 0;
    virtual int return_size() = 0;
    virtual void display() = 0;
    virtual void clear() = 0;
};
