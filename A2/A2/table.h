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
#include <functional>
#include <list>

/*************************************************************************/
/* This abstract base class defines pure virtual functions for the tasks */
/* that the various hash table implementations must perform              */ 
/*************************************************************************/
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

    //find appropriate index for insertion
    if(size() > 0){
      while ( i <size() && toInsert._key > _store[i]._key )
        i++;
    } 

    //perform insertion and rearrange data to conform with sort property
    _store.insert(_store.begin() + i, toInsert);	

    return true;

  } 

  /************************************************************************
  * Attempts to remove an item from the table, given a const reference    *
  * to the item's key - returns false if key does not exist in table			*
  ***********************************************************************/
  virtual bool remove(const Key_t& key){
    if(!find(key)) return false;

    for(int idx = 0; idx < size(); idx++){
      if(_store[idx]._key == key){
        _store.erase(_store.begin() + idx);
        return true;
      }
    }
    return true;
  }

  virtual int size() const{ return _store.size(); }

  virtual ~Simple(){
    for(int i = 0; i < size(); i++)
      _store.erase(_store.begin() + i);	
  }


};

/************************************************************************/
/* Linear Probing Hash Table implementation of abstract Table class     */
/************************************************************************/
template <typename Key_t, typename Val_t> 
class LPHash : public Table<Key_t, Val_t> {
  struct Item
  {
    Key_t _key;
    Val_t _val;
    Item(Key_t k, Val_t v) : _key(k), _val(v) {}
    Key_t key(){ return _key;}
  };
  std::vector<Item*> _store;
  int _numElements;
  std::hash<Key_t> _hash;
public:

  LPHash(int maxExpected) : _numElements(0){  
    if(maxExpected < 10)
      maxExpected = 10;
    _store.resize((int)(1.25 * maxExpected));
  }

  /*********************************************************************/
  /* Probes table for the key specified in a passed const reference &  */
  /* returns either a pointer the value at this location or a nullptr  */ 
  /*********************************************************************/
  virtual Val_t* find(const Key_t& key){

    // find ideal key location
    int end = _store.size();
    size_t ideal = _hash(key) % end;

    // probe table for specified key
    for(int i = 0; i < end; i++ ){
      int idx = (ideal + i) % end;
      // break if an island is found
      if(!_store[idx])
        return nullptr;
      // return the specified value
      if(_store[idx]->_key == key)
        return &_store[idx]->_val;        

    }

    return nullptr;
  }

  /************************************************************************/
  /* Attempts to insert a new item into the table, given const references */
  /* to a key and a value - returns false if key exists in table          */ 
  /************************************************************************/
  virtual bool insert(const Key_t& key, const Val_t& val) {                  
    if (find(key)) return false;    

    int end = _store.size();
    size_t ideal = _hash(key) % end;
    int insertion = -1;

    //locate ideal index of insertion
    for(int i = 0; i < end && insertion < 0; i++){
      int idx = (ideal + i) % end;
      if(!_store[idx])
        insertion = idx;    
    }

    //perform insertion and increment load
    _store[insertion] = new Item(key, val);
    _numElements++;

    //rehash when load factor exceeds tolerance level
    if ( (1.25 * _numElements) >= _store.size())
      rehash();

    return true;

  }

  /***********************************************************************
  * Attempts to remove an item from the table, given const references    *
  * to a key and a value - returns false if key does not exist in table  *
  ************************************************************************/
  virtual bool remove(const Key_t& key) {

    if(!find(key)) return false;

    int end = _store.size();
    auto ideal = _hash(key) % end; 
    Item* toDel = _store[ideal];
    int removal = -1;

    //locate ideal index of removal
    for(int i = 0; i < end && removal < 0; i++ ){
      int idx = (ideal + i) % end;
      if(_store[idx] == NULL)
        return false;
      if(_store[idx]->_key == key)
        removal = idx;
    }

    //perform removal
    delete _store[removal];
    _store[removal] = nullptr;

    int oldRemovalPos = removal;

    //fix 'island' generated by newly emptied position
    for(int i = 1; i < end; i++){
      //offset from the position deleted from
      int idx = (oldRemovalPos + i) % end;      

      //the end of the island has been encountered
      if (!_store[idx]) break;

      //ideal position for key at current idx
      size_t iPos = _hash(_store[idx]->_key)%end;      

      //decide if current Item's position should
      //be swapped with removal index
      if (shouldMove(idx, removal, iPos)){
        _store[removal] = _store[idx];
        _store[idx] = nullptr;        
        removal = idx;
      }

    }

    //decrement load
    _numElements--;

    return true;

  }

  /*********************************************************************/
  /* Recieves integers representing relative positions and determines  */
  /* whether remove() should perform moves after node deletion         */
  /*********************************************************************/
  bool shouldMove(int current, int removal, int ideal){

    bool rv = false;

    if(removal < current){
      if(ideal <= removal || current < ideal)
        rv = true;
    } else if(current < removal){
      if(current < ideal && ideal <= removal)
        rv = true;
    }

    return rv;

  }

  /**************************************************/
  /*Returns current tally of elements in data store */
  /**************************************************/
  virtual int size() const { return _numElements; }

  /***********************************************************************/
  /* Resizes data store and, through probing, inserts current            */
  /* data set into new ideal indices                                     */
  /***********************************************************************/
  void rehash(){

    //initialise temporary datastore
    int newCap = (int)(_store.size() * 1.25);
    int oldCap = _store.size();
    std::vector<Item*> resized(newCap);    

    //iterate through all store's elements and
    //perform new insertions
    for(int i = 0; i < oldCap; i++){
      int ideal=-1, insertion=-1, end = resized.size();
      if(_store[i]){
        ideal = _hash(_store[i]->_key) % newCap;                
        for(int j = 0; j < end && insertion < 0; j++){
          int idx = (ideal + j) % end ;
          if(!resized[idx])
            insertion = idx;    
        }
        resized[insertion] = _store[i];
      }
    }

    //re-initialize data store
    _store.clear();    
    _store = std::vector<Item*>((int)(oldCap *1.25));

    //assign data to new store
    _store = resized;

  }

  virtual ~LPHash(){
    for(int i = _store.size()-1; i =0 ; i--)
      remove(_store[i]->_key);
  }

};

/************************************************************************/
/* Chained Hash Table implementation of abstract Table class            */
/************************************************************************/
template <typename Key_t, typename Val_t>
class ChainHash : public Table<Key_t, Val_t> {
  struct Item
  {
    Key_t _key;
    Val_t _val;

    Item(Key_t k, Val_t v) : _key(k), _val(v){}

  };  
  std::vector<std::list<Item*>> _store;
  std::hash<Key_t> _hash;  
  int _numElements;
  static const int bucketSize = 50;
public:

  ChainHash(int maxExpected) : _numElements(0){            
    if(maxExpected < 10)
      maxExpected = 10;
    _store.resize((int)(1.25 * maxExpected)/bucketSize);    
  }

  /*********************************************************************/
  /* Iterates through all buckets for the key specified in a passed    */ 
  /* const reference and returns either a pointer the value at this    */ 
  /* location or a nullptr                                             */ 
  /*********************************************************************/
  virtual Val_t* find(const Key_t& key) {    

    size_t ideal = _hash(key) % _store.size();   

    std::list<Item*>& list = _store[ideal];

    std::list<Item*>::iterator it = list.begin();

    while(it!=list.end()){      
      if((*it)->_key == key)
        return &(*it)->_val;
      it++;
    }   

    return nullptr;

  }

  /************************************************************************/
  /* Attempts to insert a new item into the table, given const references */
  /* to a key and a value - returns false if key exists in table          */ 
  /************************************************************************/
  virtual bool insert(const Key_t& key, const Val_t& val) {

    if(find(key))return false;

    //rehash if load exceeds tolerance level
    if( (1.25 * _numElements) >= _store.size())
      rehash();

    //find ideal insertion location
    size_t ideal = _hash(key) % _store.size();
    int size = _store[ideal].size();   

    Item* toAdd = new Item(key, val);

    //perform insertion and increment 
    _store[ideal].push_back(toAdd);
    _numElements++;

    return true;

  }

  /************************************************************************/
  /* Attempts to remove an item from the table, given const references    */
  /* to a key and a value - returns false if key exists in table          */ 
  /************************************************************************/
  virtual bool remove(const Key_t& key) {

    if(!find(key)) return false;

    //find expected removal index
    auto ideal = _hash(key) % _store.size();    
    //Item* tmp = nullptr;
    std::list<Item*>& list = _store[ideal];

    //iterate through list to perform removal
    for(auto i = _store[ideal].begin(); i != _store[ideal].end(); i++){      
      if((*i)->_key == key){
        _store[ideal].erase(i);
        _numElements--;
        return true;
      }

    }

  }

  /*************************************/
  /* Returns current size of data set  */
  /*************************************/
  virtual int size() const {return _numElements;}

  virtual ~ChainHash(){
    for(int i = 0; i < _store.size(); i++)
      _store[i].clear();
  }

  /***********************************************************************/
  /* Resizes data store and, through chaining into ideal bucket lists,   */  
  /* insert current data set into new ideal locations                    */
  /***********************************************************************/
  void rehash(){    

    //initialise temporary datastore
    int newCap = _store.size() * 2;    
    int oldCap = _store.size();    
    std::vector<std::list<Item*>> resized;    
    resized.resize(newCap);

    //create teporary list item for copying values from old to new store
    std::list<Item*> list;       

    //iterate through old store, copying items to ideal locations in new store
    for(int i = 0; i < oldCap; i++){
      int ideal = -1, insertion = -1, end = resized.size();      
      list = _store[i];
      std::list<Item*>::iterator it = list.begin();     

      while(it != list.end()){        
        ideal = _hash((*it)->_key) % end;
        resized[ideal].push_back(*it);
        it++;
      }

    }

    //re-initialize data store
    _store.clear();
    _store = std::vector<std::list<Item*>>(newCap);   

    //assign copied data to new store
    _store = resized;    

  }


};



#endif
