#include "Iterator.h"
#include "Node.h"

template <typename T> class LinkedList
{
private:
  Node*<T> sentinel; 
public:
  LinkedList(){
    sentinel->next=sentinel->prev=sentinel;
  }

  LinkedList(const LinkedList& other){
    if(other && other.front()){

      Iterator o = other.begin();
      Iterator c = begin();

      for(; c && c->next() && o && o.next(); c++){

      }

    }
  }

  LinkedList& operator=(const LinkedList& other){
    this = new LinkedList(other);
  }

  Iterator begin() const{ return front(); }
  Iterator end() const{ return back()->next(); }

  T& front(){ return sentinel.next(); }
  T& back(){ return sentinel.prev(); }

  void push_back(const T& val);
  void push_front(const T& val);

  void erase(const Iterator& position{

  }

  void insert(const Iterator& position, const T& val){
    if(!position){
      push_back(val);
      return;
    } else
    {
      Node n = new Node(val);
      Node temp = position->next;
      

    }


  }

  bool empty() const{ return sentinel.next() == sentinel; }

  unsigned int size() const{
    auto size = 0;

    if(!empty(){
      Iterator c = begin();
      for(; c->next() == head; c++){
        size++;
      }
    }

    return size;

  }

  void clear(){
    while(!empty()){

    }

  }

  ~LinkedList();  
};