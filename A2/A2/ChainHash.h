#ifndef __CHAIN_H__
/*
#include "table.h"
#include <list>
#include <vector>
#include <functional>
#include <iterator>


template <typename Key_t, typename Val_t> 

class ChainHash {
  struct Item
  {
    Key_t _key;
    Val_t _val;    
  };  
  std::vector<std::list<Item>> _store;
  std::hash _hash;
  int _capacity;
  int _maxload;
  int _end;
public:

  ChainHash(int maxExpected){
    _capacity = 1.25 * maxExpected;
    _maxload = maxExpected / 10;
    _store = new std::vector<std::list<Item>>(maxExpected);    
  }

  virtual Val_t* find(const Key_t& key) {            
    auto ideal = _hash(key) % _capacity;
    
    for(std::list<Item>::iterator i = _store[ideal].begin(); i < _store[i].end(); i++){
      if(*i->_key == key)
        return &i->_val;
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
      if(*i->_key == key){
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


}; */


#define __CHAIN_H__
#endif