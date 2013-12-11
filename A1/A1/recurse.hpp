#ifndef __NM_SOLVER__
#define __NM_SOLVER__

#include <iostream>

class RecursiveSolver
{
private:
  unsigned int* _values;//make double ptrs? (2d arrays)
  bool* _visited;       //
  int _rows;
  int _cols;
  int _size;
public:

  /****************************************************************************************************/
  /* Accepts an unsigned int pointer pointing to the first element in the two dimensional data array, */
  /*         dataLen, the total number of elements in the data array, and width, the width of a row   */
  /*         in the data array; then constructs a copy of data for the application store              */
  /****************************************************************************************************/
  RecursiveSolver(const unsigned int* data, int dataLen, int width){    
    if (data){
      //extract size of data array
      _size = dataLen;
      _rows = dataLen/width;
      _cols = width;      
      
      //initialize empty arrays
      _values = new unsigned int[_size];
      _visited = new bool[_size];

      //create copy of data and an array of falses
      for (int i=0; i <= _rows; i++){
        for(int j = 0 ; j <= _cols; j++){
          _values[(i*j) + j ] = data[(i*j) + j];     
          _visited[(i*j) + j] = false;
        }
      }
      
    }
  }

  /*****************************************************************************/
  /* Returns the sum of the group that contains the cell located at (row,col)  */
  /* whose values are above lowerBound and below upperBound                    */
  /*****************************************************************************/
  unsigned int sum(unsigned int lowerBound, unsigned int upperBound, int row, int col){
    
    clearVisited();

    return recursiveSum(lowerBound, upperBound, row, col);    

  }

  /****************************************************************************************/
  /* Returns the number of groups whose values are above lowerBound and below upperBound  */
  /****************************************************************************************/
  unsigned int count(unsigned int lowerBound, unsigned int upperBound){

    clearVisited();

    int sum = 0, _count = 0;

    for (int rows = 0 ; rows <= _rows; rows++){
      for(int cols = 0; cols <= _cols; cols++){
        sum = recursiveSum(lowerBound, upperBound, rows, cols);

        if (sum > 0)
          _count++;
      }
    }

    return _count;

    
  }

  unsigned int recursiveSum(unsigned int lowerBound, unsigned int upperBound, int row, int col){

    if(row > 0 && row <= _rows && col > 0 && col <= _cols && !_visited[row * col]){

      _visited[(row * col) + col] = true;
      
      unsigned int val = _values[(row * col) + col];

      if(val > lowerBound && val < upperBound)
        return val + recursiveSum(lowerBound, upperBound, row-1, col) 
        + recursiveSum(lowerBound, upperBound, row+1, col)
        + recursiveSum(lowerBound, upperBound, row, col-1)
        + recursiveSum(lowerBound, upperBound, row, col+1);

    }else
      return 0;
      
  }

  void clearVisited(){

    for(int i = 0; i <= _size ; i++)
      _visited[i] = false;

  }

  
  ~RecursiveSolver(){}
};


#endif