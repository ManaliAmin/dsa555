#ifndef __NM_SIMPLE_H__

#include "table.h"
#include <vector>

template <typename Key_t, typename Val_t>
class Simple : public Table<Val_t>
{
private:
  struct Item
  {
    Key_t _key;
    Val_t _val;
  };
  std::vector<Item> _store;
  int _size;
public:
  Simple(int initialCapacity){
    _size = initialCapacity;
    _store = new std::vector(_size);
  }

  virtual Val_t* find(const Key_t& key){

    if(size() == 0) return nullptr;

    int min=0, max = size(), mid = max/2;

    while(max >= min){
      if (_store[mid]->_key < key)
        min = mid+1;
      else if (_store[mid]->_key > key)
        max = mid-1;
      else
        return &mid->_val;
    }
  }

  virtual bool insert(const Key_t& key, const Val_t& val){
    if(find(key)) return false;

    if(_store[size()])
      _store.resize(2*_size);
        

    Item toInsert = new Item(key, val);
    Item tmp = nullptr;

    _store.push_back(toInsert);

    iterator i = _store.begin();    
    while (_store[i]->_key < key)
      i++;
    
    tmp = _store[i];
    
    _store[i] = toInsert;    

    for( i = i+1 ; i < _store.end() ; i++){
      _store[i+1] = _store[i];

   }  
        
    if(_store[size()]){
      _store.resize(2* _size);       


  }

  void sort(int startIndex, int endIndex)
  {
    int pivot = array[startIndex];                 
    int splitPoint;

    if(endIndex > startIndex)                        
    {
      splitPoint = SplitArray(array, pivot, startIndex, endIndex);
      _store[splitPoint] = pivot;
      sort(startIndex, splitPoint-1);  
      sort(splitPoint+1, endIndex);   
    }
  }
 

  virtual bool remove(const Key_t& key){
    if(!find(key)) return false;

    int idx = 0;
    bool rv = true;
    Item toDel = nullptr;

    for(; idx < size() & _store[idx]->key < key ; idx++);

    toDel = _store[idx];

    do{
      _store[idx] = _store[idx+1];
    }while (idx < size());

    delete _store[size()];
    delete toDel;
    return true;
  }
  virtual int size() const{ return _store.size(); }

  virtual ~Simple(){
    for(std::iterator i = _store.end(); i = _store.begin(); i--)
      delete _store[i];
    delete this;
  }


};



#define __NM_SIMPLE_H__
#endif