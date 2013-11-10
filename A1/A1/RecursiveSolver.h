#ifndef __NM_SOLVER__
  
  class RecursiveSolver
  {
  public:
    RecursiveSolver(const unsigned int* data, int dataLen, int width){
     arr = new char[dataLen+width];
     if (data)
       for (int i=0; i < sizeof(data)/sizeof(unsigned int); i++)
         arr[i] = data[i];       
    }



    ~RecursiveSolver();

  private:
    char* arr;
  };



  #define __NM_SOLVER__
#endif