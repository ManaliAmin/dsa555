#ifndef __TABLE_H__
#define __TABLE_H__

/*Student Assignment Submission Form
==================================
I/we declare that the attached assignment is my/our own work in accordance with
Seneca Academic Policy.  No part of this assignment has been copied manually or 
electronically from any other source (including web sites) or distributed to other 
students.

Name(s)                                             Student ID(s)

Natesh Mayuranathan               							    046643086

*/

#include <vector>
#include <iterator>
#include <functional>
#include <list>
#include <iostream>

/************************************************************************/
/* This abstract base class defines pure virtual functions for the tasks
* that the various hash table implementations must perform
************************************************************************/
template <typename Key_t, typename Val_t> class Table {  
public:
  virtual Val_t* find(const Key_t& key) = 0;
  virtual bool insert(const Key_t& key, const Val_t& val) = 0;
  virtual bool remove(const Key_t&) = 0;
  virtual int size() const = 0;
  virtual ~Table(){}
};

/************************************************************************/
/* Simple Array Table implementation of abstract Table class            */
/************************************************************************/
template <typename Key_t, typename Val_t>
class Simple : public Table<Key_t, Val_t>
{
private:
  struct Item
  {
    Key_t _key;
    Val_t _val;

    Item(Key_t k, Val_t v): _key(k), _val(v){}
  };
  std::vector<Item> _store;
public:

  Simple(int initialCapacity){
    _store.reserve(initialCapacity);
  }

  /************************************************************************
  * Performs a binary search of the sorted array table that returns      * 
  * either the value associated with the key requested or a nullptr      *
  * (not found)                                                          *  
  ************************************************************************/
  virtual Val_t* find(const Key_t& key){

    if(size() == 0) return nullptr;

    int min=0, max = size(), mid=0;

    while(max >= min){
      mid = min + (max-min)/2;
      if (mid==size()) break;
      if (_store[mid]._key < key)
        min = mid+1;
      else if (_store[mid]._key > key)
        max = mid-1;
      else
        return &_store[mid]._val;
    }

    return nullptr;

  }

  /************************************************************************
  * Attempts to insert a new item into the table, given const references *
  * to a key and a value - returns false if key exists in table
  ************************************************************************/
  virtual bool insert(const Key_t& key, const Val_t& val){
    if(find(key)) return false;

    Item toInsert(key, val);

    int i = 0;    
    if(size() > 0){
      while ( i <size() && toInsert._key > _store[i]._key )
        i++;
    } 

    _store.insert(_store.begin() + i, toInsert);	

    return true;

  } 

  /************************************************************************
  * Attempts to remove an item from the table, given a const reference    *
  * to the item's key - returns false if key does not exist in table			*
  ***********************************************************************/
  virtual bool remove(const Key_t& key){
    if(!find(key)) return false;

    int idx = 0;
    bool rv = true;

    for(; idx < size() && _store[idx]._key < key ; idx++);

    _store.erase(_store.begin() + idx);

    return true;
  }

  virtual int size() const{ return _store.size(); }

  virtual ~Simple(){
    for(int i = 0; i < size(); i++)
      _store.erase(_store.begin() + i);	
  }


};

template <typename Key_t, typename Val_t> 
class LPHash : public Table<Key_t, Val_t> {
  struct Item
  {
    Key_t _key;
    Val_t _val;
  };
  std::vector<Item> _store;
  int _load;
  std::hash<Key_t> _hash;
public:

  LPHash(int maxExpected) : _load(0){    
    _store.reserve(1.25 * maxExpected);

  }

  virtual Val_t* find(const Key_t& key){

    int end = _store.size();

    auto ideal = _hash(key) % end;
    for(int i = 0; i < end && &_store[(i+1) % end]; i++ ){
      if(_store[(ideal + i) % end]._key == key){
        return &_store[(ideal + i) % end]._val;
      }
    }

    return nullptr;
  }
  virtual bool insert(const Key_t& key, const Val_t& val) {        

    if (find(key)) return false;    
    int end = _store.size();
    auto ideal = _hash(key) % end;
    int insertion = -1;

    if(_store[ideal]){
      for(int i = ideal+1; i < _store.size() && insertion < 0; i++){
        if(!_store[i])
          insertion = i;
      }
      if (i == _store.end()){
        for(i = 0; i < ideal; i++){
          if(!_store[i])
            insertion = i;
        }    
      }
    } else
      insertion = ideal;


    _store[insertion] = new Item(_key, _val);
    _load++;
    if ( size() == (1.25 * _load))
      rehash();

    return true;

  }
  virtual bool remove(const Key_t& key) {

    if(!find(key)) return false;

    int end = store.size();
    auto ideal = _hash(key) % end;
    //Item temp = nullptr;    
    int removal = -1;
    int i=-1;

    if(_store[ideal]._key == key)
      removal = ideal;
    else
    {
      for(i = ideal; i < _store.end() && removal < 0 ; i++ ){
        if(_store[i]._key == key)
          removal = i;
      }

      if(i == _store.end()){
        for(i = 0 ; i < ideal; i++){
          if(_store[i]._key == key)
            removal = i;
        }
      }
    }

    delete _store[removal];
    _load--;
    return true;


  }
  virtual int size() const { return _store.size(); }

  void rehash(){
    std::vector<Item> resized = new std::vector<Item>(2 * _size);
    for(int i = 0; i < _store.size(); i++)
      resized[i] = _store[i];
  }

  virtual ~LPHash(){
    for(int i = store.size()-1; i =0 ; i--)
      remove(store[i]._key);

    delete[] _store;
  }

};

template <typename Key_t, typename Val_t>
class ChainHash : public Table<Key_t, Val_t> {
  struct Item
  {
    Key_t _key;
    Val_t _val;    
  };  
  std::vector<std::list<Item>> _store;
  std::hash<Key_t> _hash;
  //maximum elements in table
  int _capacity;
  //maximum acceptable load of each bucket
  int _maxload;
  int _end;
  int _buckets;
public:

  ChainHash(int maxExpected){
    _capacity = 1.25 * maxExpected;
    _buckets = 10;
    _maxload = maxExpected / _buckets;
    _store = new std::vector<std::list<Item>>(maxExpected);    
  }

  virtual Val_t* find(const Key_t& key) {            
    auto ideal = _hash(key) % _capacity;

    for(std::list<Item>::iterator i = _store[ideal].begin(); i < _store[i].end(); i++){
      if(*i._key == key)
        return &i._val;
    }

    return nullptr;

  }
  virtual bool insert(const Key_t& key, const Val_t& val) {
    if(find(key))return false;

    auto ideal = _hash(key) % _capacity;
    int size = _store[ideal].size();

    _store[ideal].push_back(new Item(key, val));

    if(_store[ideal].size() == _maxload)
      resize();


  }
  virtual bool remove(const Key_t& key) {

    if(!find(key)) return false;

    auto ideal = _hash(key) % _capacity;
    int size = _store[ideal].size();
    Item tmp = nullptr;

    for(std::list<Item>::iterator i = _store[ideal].begin(); i < _store[i].end(); i++){
      if(*i._key == key){
        tmp = *i;
        *i-1

      }

    }

  }
  virtual int size() const {return _capacity;}
  virtual ~ChainHash(){
    for(int i = 0; i < _capacity; i++)
      _store[i].clear();
  }

  void resize(){    
    int expanded = _capacity * 2;
    std::vector<std::list<Item>> resized = new std::vector<std::list<Item>>(expanded);    
    _maxload = (expanded/1.25)/10;

    for(int i = 0; i < _capacity; i++){
      if(_store[i].begin())
        resized[i] = _store[i];
    }

  }


};



#endif
