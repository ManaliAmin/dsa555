#ifndef __NMAYURANATHAN_LIST__
#define __NMAYURANATHAN_LIST__

#include <stddef.h>

template<typename T> class Node;
template<typename T> class Iterator;
template<typename T> class LinkedList;

/*******************************/
/* Templated linked list node  */
/*******************************/
template<typename T>
class Node
{
private:
  T _data;
  Node* _next;
  Node* _prev;
public:

  /***********************/
  /* Default constructor */      
  /***********************/

  Node() : _data(){    
    _next = _prev = nullptr;
  }

  /************************/
  /* Argument constructor */
  /************************/
  Node(T v){
    _data = v;    
    _next = _prev = NULL;
  }

  /*********************************************/
  /* Returns a pointer to next/previous nodes  */
  /*********************************************/
  Node<T>* next(){ return _next; }   
  Node<T>* prev(){ return _prev; }

  /****************************************/
  /* Returns a reference to the data held */
  /* by the node                          */ 
  /****************************************/
  T& data(){ return this->_data; }

  /**********************************************/
  /* Assigns values to current node's next and  */
  /* previous node pointers                     */
  /**********************************************/
  void setNext(Node<T>* n) { _next = n; }
  void setPrev(Node<T>* n) { _prev = n; }

  friend class LinkedList<T>;

};

template <typename T>
class Iterator{

private:
  Node<T>* _target;
public:

  Iterator(){ _target = NULL; }

  Iterator(const Iterator& src){
    _target = src._target;
  }

  Iterator& operator=(const Iterator& src){
    _target = src._target;
    return *this;
  }

  Iterator& operator++(){    
    _target = _target->next();
    return *this;
  }

  Iterator operator++(int n){ 
    Iterator i = *this;
    _target = _target->next();
    return i;
  }

  Iterator& operator--(){
    _target=_target->prev();
    return *this;
  }

  Iterator& operator--(int n){
    Iterator i = *this;
    _target = _target->prev();
    return i;
  }

  bool operator==(const Iterator& other) const{
    return _target == other._target;
  }

  bool operator!=(const Iterator& other) const{
    return _target != other._target;
  }

  T& operator*(){
    return _target->data();
  }

  T* operator->(){
    return &_target->data();
  }

  friend class LinkedList<T>;

};

/**************************************************/
/* Linked List of templated nodes using iterators */
/* for traversal                                  */
/**************************************************/
template <typename T> class LinkedList
{
private:
  Node<T>* _sentinel;
  int _size;
public:
  /***********************/
  /* Default constructor */
  /***********************/
  LinkedList() : _size(0){
    _sentinel = new Node<T>();
    _sentinel->setNext(_sentinel);
    _sentinel->setPrev(_sentinel);
  }

  /********************/
  /* Copy constructor */
  /********************/  
  LinkedList(const LinkedList& other){
    *this = other;
  }

  /***********************/
  /* Assignment operator */
  /***********************/
  LinkedList& operator=(const LinkedList& other){

    if(*this->_sentinel != other->_sentinel){
      for(auto i = other.begin(); i != other.end(); i++)
        push_back(*i);
    }

    return *this;
  }

  /************************************************/
  /* Returns an iterator to the front of the list */
  /************************************************/
  Iterator<T> begin() const{ 
    Iterator<T> i;
    i._target = _sentinel->next();
    return i;
  }

  /***********************************************/
  /* Returns an iterator to the back of the list */
  /***********************************************/
  Iterator<T> end() const{ 
    Iterator<T> i;   
    i._target = _sentinel->prev();
    return i;
  }

  T& front() const{ return _sentinel->next()->data(); }
  T& back() const{ return _sentinel->prev()->data(); }

  /**********************************************/
  /* Inserts a new value at the end of the list */
  /**********************************************/
  void push_back(const T& val){

    Node<T>* n = new Node<T>(val);
    n->setPrev(_sentinel->prev());
    _sentinel->prev()->setNext(n);
    n->setNext(_sentinel);
    _size++;
  }

  /****************************************************/
  /* Inserts a new value at the beginning of the list */
  /****************************************************/
  void push_front(const T& val){
    Node<T>* n = new Node<T>(val);
    n->setNext(_sentinel->next());
    _sentinel->next()->setPrev(n);    
    _sentinel->setNext(n);
    _size++;
  }

  /************************************************************/
  /* Removes a new value from a specific position in the list */
  /************************************************************/
  void erase(const Iterator<T>& position){   
    Node<T>* toDel = position._target;

    if(toDel){
      toDel->next()->setPrev(toDel->prev());
      toDel->prev()->setNext(toDel->next());
      delete toDel;
      _size--;
    }   

  }

  void insert(const Iterator<T>& position, const T& val){

    if(position == begin())
      push_front(val);
    else if(position == end())
      push_back(val);
    else{

      Node<T>* curr = position._target; 
      Node<T>* toAdd = new Node<T>(val);

      toAdd->setPrev(curr->prev());
      curr->prev()->setNext(toAdd);
      toAdd->setNext(curr);
      curr->prev()->setNext(toAdd);
      _size++;
    }
  }

  bool empty() const{ return !size(); }

  unsigned int size() const{ return _size; }

  void clear(){    
    while(!empty()){
      erase(begin());     
    }

  }

  ~LinkedList(){
    clear();    
  }
};

#endif