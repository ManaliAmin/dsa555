#include "A3Interfaces.hpp"

class WordNode : public iWordNode{
private:  
  WordNode* children[26];	
  size_t _numChildWords;
  bool _isWholeWord;
protected:

public:
  WordNode() : _numChildWords(0), _isWholeWord(false){    
    for(int i = 0; i < 26; i++)
      children[i]=nullptr;
  }  

  /**
  * Attempts to insert a character in the appropriate
  * position
  */
  void insert(WordNode* curr, const char* word){

    if(!word)
      return;

    //Get leading char from word and extract ASCII character
    char c = word[0];
    int code = c - 97;

    //perform insertion at appropriate node, provided it is empty
    if(!curr->children[code])
      curr->children[code] =  new WordNode();
    //set complete word marker as necessary
    if(!word[1]){
      curr->children[code]->_isWholeWord = true;
      curr->_numChildWords++;
    }
    else{
      //continue to insert remaining characters in word
      insert(curr->children[code], word+1);      
    }    

  }

  /**
  Returns a pointer to an iWordNode instance at the given letter.
  If none exist, this function should return nullptr.
  */
  virtual WordNode* getChild(char letter){    
    int pos = letter - 97;

    if(children[pos])
      return children[pos];

    return nullptr;
  }  

  WordNode* find(WordNode* curr, const char* wordSegment){

    if(!curr || !wordSegment) return nullptr;

    int code = wordSegment[0];

    if(curr->getChild(code)){
      if(!wordSegment[1])
        return curr->getChild(code);
      return find(curr->getChild(wordSegment[0]), wordSegment+1);
    } else  
      return nullptr;

  }

  /**
  Returns the total number of children that this node and all of its
  subtrees contain.
  */
  virtual size_t getNumChildWords() const{ 

    size_t total = 0;

    for(int i=0; i < 26;i++){
      if(children[i] && children[i]->isWholeWord())
        total++;
      total+=getTotChildWords(children[i], total);
    }

    return total;

  }  

  size_t getTotChildWords(WordNode* curr, int recursiveTotal) const{   

    if(!curr)
      return 0;

    for(int i=0; i < 26;i++){
      if(children[i] && children[i]->isWholeWord())
        recursiveTotal++;
      recursiveTotal+=getTotChildWords(children[i], recursiveTotal);
    }

    return recursiveTotal;


  }

  /**
  Returns true if this instance is at the end of a whole word,
  false otherwise.
  */
  virtual bool isWholeWord() const{ 
    if(this)
      return _isWholeWord; 
  }

  void remove(WordNode* curr){

    if(!curr)
      return;

    for(int i=0; i<26 ; i++)
      remove(children[i]);

    delete[] children;


  }

  /**
  Virtual destructor.
  */
  virtual ~WordNode(){
    remove(this);
  }

};

class DictionaryTree
{
private:
  WordNode _root;
public:
  DictionaryTree(){

  }

  /**
  Inserts a "whole word" into the tree.
  If the word is not already in the tree, the required word node chain
  is created and stored appropriately.
  If the word is already in the tree but not marked as a whole word,
  it is marked as a whole word.
  */
  virtual void insert(const char* word){

    if (!word)
      return;

    WordNode* r = getRoot();   

    //call recursive function to perform nodal insertion
    r->insert(r, word);

  }


  /**
  Attempts to find the node associated with a given word.
  A word is found by descending character by character into the word node
  tree from the root.
  If the word is not found in the tree, nullptr is returned.
  If the word is found, a pointer to it is returned.
  */
  virtual WordNode* getNodeForWord(const char* word){    

    return getRoot()->find(getRoot(), word);

  }

  /**
  Simply returns the root node of the tree.
  */
  virtual WordNode* getRoot(){ return &_root; }

  virtual ~DictionaryTree(){
    getRoot()->remove(getRoot());
  }


};

