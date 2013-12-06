#ifndef __NM_NODE_H

#include <stddef.h>

template<typename T>
class Node
{
private:
  T* _data;
  Node* _next;
  Node* _prev;
public:
  Node(){    
    _next = _prev = NULL;
  }
  Node(T v){
    data = v;    
    _next = _prev = NULL;
  }

  Node<T>* next(){ return _next; } 
  Node<T>* prev(){ return _prev; }
  T& data(){ return _data; }

  void setNext(Node<T>* n) { _next = n; }
  void setPrev(Node<T>* p) { _prev = p; }
};

#define __NM_NODE_H
#endif