#ifndef __NM_LP_H__
/*
#include <vector>
#include "table.h"

template <typename Key_t, typename Val_t> 
class LPHash {
  struct Item
  {
    Key_t _key;
    Val_t _val;
  };
  std::vector<Item> _store;
  int _load;
  int _size;
  std::hash<Key_t> _hash;
public:

  LPHash(int maxExpected){    
    _size = 1.25 * maxExpected;
    _store = new std::vector<Item>(_size);
  }

  virtual Val_t* find(const Key_t& key){

    int end = _store.size();

    auto ideal = _hash(key) % end;
    for(int i = 0; i < end && _store[(ideal +1) % end]; i++ ){
      if(_store[(ideal + i) % end]->_key == key)
        return &_store[(ideal + i) % end] -> _val;
    }

    return nullptr;
  }
  virtual bool insert(const Key_t& key, const Val_t& val) {        

    if (find(key)) return false;    
    int end = _store.size();
    auto ideal = _hash(key) % end;
    int insertion = -1;

    if(_store[ideal]){
      for(int i = ideal+1; i < _store.end() && insertion < 0; i++){
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

      if(_store[ideal]->_key == key)
        removal = ideal;
      else
      {
        for(i = ideal; i < _store.end() && removal < 0 ; i++ ){
          if(_store[i]->_key == key)
            removal = i;
        }

        if(i == _store.end()){
          for(i = 0 ; i < ideal; i++){
            if(_store[i]->_key == key)
              removal = i;
          }
        }
      }

      delete _store[removal];
      _load--;
      return true;


  }
  virtual int size() const { return _size; }

  void rehash(){
    //TODO
  }

  virtual ~LPHash(){
    for(int i = store.size()-1; i =0 ; i--)
      remove(store[i]->_key);

    delete[] _store;
  }

};
*/

#define __NM_LP_H

#endif