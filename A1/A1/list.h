#ifndef __NMAYURANATHAN_LIST__

#include <stddef.h>

template<typename T> class Node;
template<typename T> class Iterator;
template<typename T> class LinkedList;

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

  friend class LinkedList<T>;

};

template <typename T>
class Iterator{

private:
  Node<T>* p;
public:
  Iterator(){ p = NULL; }
  Iterator(const Iterator& src){
    p = src->p;
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
    p = p->next();
    return i;
  }

  Iterator& operator--(){
    p=p->prev();
    return this;
  }

  Iterator& operator--(int){
    Iterator i = this;
    p = p->prev();
    return i;
  }

  bool operator==(const Iterator& other) const{
    return p == other.p;
  }
  bool operator!=(const Iterator& other) const{
    return p != other.p;
  }

  T& operator*(){
    return p->data;
  }

  T* operator->(){
    return &p->data;
  }

  void set(T& data){
    p = data;
  }

  friend class LinkedList<T>;

};

template <typename T> class LinkedList
{
private:
  Node<T>* sentinel;
public:
  LinkedList(){
    sentinel->setNext(sentinel);
    sentinel->setPrev(sentinel);
  }

  LinkedList(const LinkedList& other){
    *this = other;
  }

  LinkedList& operator=(const LinkedList& other){
    clear();
    sentinel = other.sentinel;
  }

  Iterator<T> begin() const{ 
    Iterator<T> i;   
    i.set(front());
    return i;
  }

  Iterator<T> end() const{ 
    Iterator<T> i;   
    i.set(back());
    return i;
  }

  T& front() const{ return &sentinel.next(); }
  T& back() const{ return &sentinel.prev(); }

  void push_back(const T& val);
  void push_front(const T& val);

  void erase(const Iterator<T>& position){   
    

  }

  void insert(const Iterator<T>& position, const T& val){
    if(!position){
      push_back(val);
      return;
    } else
    {
      Node<T> n = new Node(val);
      Node<T> temp = position->next;
      

    }


  }

  bool empty() const{ return sentinel.next() == sentinel; }

  unsigned int size() const{
    auto size = 0;
    Iterator<T> c = begin();

    if(!empty()){      
      for(; c->next() == head; c++){
        size++;
      }
    }

    return size;

  }

  void clear(){
    Iterator<T> i = begin();
    while(!empty()){
      Iterator<T> toDel = i++;
      delete toDel.p;      
    }

  }

  ~LinkedList(){
    clear();    
  }
};

#define __NMAYURANATHAN_LIST__
#endif