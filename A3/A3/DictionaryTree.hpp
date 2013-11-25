#include "A3Interfaces.hpp"

#include <vector>

class WordNode : public iWordNode{
private:
	char _letter;
  std::vector<WordNode*> children;	
	size_t _numChildren;
	bool _isWholeWord;
public:
  WordNode(){
		children.reserve(26);	
	}


	/**
	 * Attempts to insert a word's individual characters
	 * into the node and its chidlren as needed
	 */
	virtual void insert(const char* word){
		int indices = getNumChildren();
		int sz = word.size();
		for(int i = 0; i < sz ; i++){
			if(!children[atoi(word[i])])

		}
	}	

  /**
  Returns a pointer to an iWordNode instance at the given letter.
  If none exist, this function should return nullptr.
  */
  virtual WordNode* getChild(char letter){
		int sz = getNumChildren();
		for(int i = 0 ; i < sz; i++){
			if(children[i]._letter == letter)
				return &children[i];
		}
		return nullptr;
	}


  /**
  Returns the total number of children that this node and all of its
  subtrees contain.
  */
  virtual size_t getNumChildren() const{ return _numChildren; }

  /**
  Returns true if this instance is at the end of a whole word,
  false otherwise.
  */
  virtual bool isWholeWord() const{ return _isWholeWord; }

  /**
  Virtual destructor.
  */
  virtual ~WordNode(){
		
	}


};

class DictionaryTree
{
private:
  WordNode* _root;
public:
  DictionaryTree(){
		_root = new WordNode();
	}

  /**
  Inserts a "whole word" into the tree.
  If the word is not already in the tree, the required word node chain
  is created and stored appropriately.
  If the word is already in the tree but not marked as a whole word,
  it is marked as a whole word.
  */
  virtual void insert(const char* word){
		
	}

  /**
  Attempts to find the node associated with a given word.
  A word is found by descending character by character into the word node
  tree from the root.
  If the word is not found in the tree, nullptr is returned.
  If the word is found, a pointer to it is returned.
  */
  virtual WordNode* getNodeForWord(const char* word){}

  /**
  Simply returns the root node of the tree.
  */
  virtual WordNode* getRoot(){ return _root; }

  virtual ~DictionaryTree(){}


};

