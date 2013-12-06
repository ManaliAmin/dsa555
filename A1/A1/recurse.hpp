#ifndef __NM_SOLVER__
  
  class RecursiveSolver
  {
  private:
    unsigned int* arr;
  public:
    RecursiveSolver(const unsigned int* data, int dataLen, int width){
     arr = new unsigned int[dataLen+width];
     if (data)
       for (int i=0; i < sizeof(data)/sizeof(unsigned int); i++)
         arr[i] = data[i];       
    }

    /*
      Returns the sum of the group that contains the cell located at (row,col) whose values are above lowerBound and below upperBound
    */
    unsigned int sum(unsigned int lowerBound, unsigned int upperBound, int row, int col){
      
    }

    unsigned int count(unsigned int lowerBound, unsigned int upperBound){}



    ~RecursiveSolver();
  };



  #define __NM_SOLVER__
#endif