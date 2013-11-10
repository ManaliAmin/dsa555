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
  Iterator operator++(int){ 
    Iterator i = this;
    p = p.next();
    return i;
  }

  Iterator& operator--(){
    p=p.prev();
    return this;
  }

  Iterator& operator--(int){
    Iterator i = this;
    p = p.prev();
    return i;
  }

  bool operator==(const Iterator& other) const{
    return *p = other;
  }
  bool operator!=(const Iterator& other) const{
    return *p != other;
  }

  T& operator*(){
    return *p;
  }

  T* operator->(){
    return p;
  }

};

#define __NM_ITER_H
#endif