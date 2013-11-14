#ifndef __TABLE_H__
#define __TABLE_H__

// This is a minimal implementation of table.h
// Feel free to use this one as a starting point or to write your own.
// Also, please remove this comment block and replace it with something more
// appropriate if you end up using this definition.
template <typename Key_t, typename Val_t> class Table {
  /*struct Item
  {
    Key_t _key;
    Val_t val;
  };*/
public:
    virtual Val_t* find(const Key_t& key) = 0;
    virtual bool insert(const Key_t& key, const Val_t& val) = 0;
    virtual bool remove(const Key_t&) = 0;
    virtual int size() const = 0;
    virtual ~Table(){}
};

#endif
