#include "A3Interfaces.hpp"

/*Student Assignment Submission Form
==================================
I/we declare that the attached assignment is my/our own work in accordance with
Seneca Academic Policy.  No part of this assignment has been copied manually or 
electronically from any other source (including web sites) or distributed to other 
students.

Name(s)                                             Student ID(s)

Natesh Mayuranathan               							    046643086

*/

class WordNode : public iWordNode{
private:  
  WordNode* children[26];	
  size_t _numChildWords;
  bool _isWholeWord;
protected:

public:
  /**
  * Default constructor - initialises node into usable state
  */
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

  /**
    Recursively searches a subtree for all the characters 
    in a passed word
  */
  WordNode* find(WordNode* curr, const char* wordSegment){

    if(!curr || !wordSegment) return nullptr;

    int code = wordSegment[0];

    if(curr->getChild(code)){
      //base case -> return WordNode* to final character in sequences
      if(!wordSegment[1])
        return curr->getChild(code);
      //recursive case -> continue to search using further indices in word segment
      return find(curr->getChild(wordSegment[0]), wordSegment+1);
    } else  
      return nullptr;

  }

  /**
    Returns the total number of children that this node and all of its
    subtrees contain.
  */
  virtual size_t getNumChildWords() const{ 
    return getTotChildWords(this);
  }  

  /**
    Recursively traverse subtree and retrieve total of word ending nodes
  **/
  static size_t getTotChildWords(const WordNode* curr){
    if(!curr)
      return 0;

    int recursiveTotal = 0;

    for(int i=0; i < 26;i++){
      const WordNode* child = curr->children[i];
      if (child) {
        recursiveTotal += child->_isWholeWord;
        recursiveTotal += getTotChildWords(child);
      }
    }

    return recursiveTotal;
  }

  /**
     Returns true if this instance is at the end of a whole word,
     false otherwise.
  */
  virtual bool isWholeWord() const{ 
      return _isWholeWord; 
  }

  /**
  Virtual destructor.
  */
  virtual ~WordNode(){
    for(int i=0; i<26 ; i++){
      if(children[i]) {
        delete children[i];
        children[i] = NULL;
      }
    }  
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

    WordNode* r = getRoot();

    //call recursive function to find word
    return r->find(r, word);

  }

  /**
  Simply returns the root node of the tree.
  */
  virtual WordNode* getRoot(){ return &_root; }

  virtual ~DictionaryTree(){}
};

