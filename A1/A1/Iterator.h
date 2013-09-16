#ifndef __NM_ITER_H

#include "Node.h"

template <typename T>
class Iterator{

private:
  Node<T>* p;
public:
  Iterator(){ p = nullptr; }
  Iterator(const Iterator& src){
    this = src;
  }
  Iterator& operator=(const Iterator& src){
   return this=src; 
  }
  Iterator& operator++(){    
    p=p.next();
    return this;
  }
  Iterator operator++(int);
  Iterator& operator--();
  Iterator& operator--(int);
  bool operator==(const Iterator& other) const;
  bool operator+=(const Iterator& other) const;
  T& operator*();
  T* operator->();

};

#define __NM_ITER_H
#endif