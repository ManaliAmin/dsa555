#ifndef __NM_NODE_H

template<typename T>
class Node
{
private:
  T* data;
  Node* next;
  Node* prev;
public:
  Node(){
    data = nullptr;
    next = prev = nullptr;
  }
  ~Node(){
  }

  Node<T>* next(){ return next; } 
  Node<T>* prev(){ return prev; }
  T& data(){ return data; }
};

#define __NM_NODE_H
#endif