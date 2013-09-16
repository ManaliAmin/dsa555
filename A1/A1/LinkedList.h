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

  LinkedList(const LinkedList& other);

  LinkedList& operator=(const LinkedList& other);

  Iterator begin() const;

  Iterator end() const;

  T& front(){ return sentinel.next(); }
  T& back(){ return sentinel.prev(); }

  void push_back(const T& val);
  void push_front(const T& val);

  void insert(const Iterator& position, const T& val);

  bool empty() const{ return sentinel.next() == sentinel; }

  unsigned int size() const;

  void clear();

  ~LinkedList();  
};