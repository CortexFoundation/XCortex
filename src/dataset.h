#ifndef XCORTEX_DATASET_H
#define XCORTEX_DATASET_H

#include "common.h"

namespace XCortex{
  struct DataSet{
    uint32_t size;
    std::vector<int32_t> data;
    
    DataSet(const uint32_t size){
      this->size = size;
      this->data.resize(size);
      this->data = xcortex_random.generate_random_value(size);
    }
    void Read(std::vector<int32_t>& vec, const uint32_t n){
      //std::copy_n(data.begin() + start, n, vec.begin()); 
      for(int i = 0; i < n; i++){
        int index = xcortex_random.generate_random_value(0, DATA_SET_SIZE);
        vec[i] = data[index];
      }
    }
    void Write(const int32_t* vec, const uint32_t n){
      //std::copy_n(vec, n, data.begin() + start);
      for(int i = 0; i < n; i++){
        //uint32_t index = (vec[i] < 0 ? -vec[i] : vec[i]) & DATA_SET_MASK;
	uint32_t index = (vec[i] & 0x7fffffff) & DATA_SET_MASK;
        data[index] = vec[i];
      }
    }
    void print(){
#ifdef PRINT_DATA_SET
      for(int i = 0; i < size; i++){
        std::cout << data[i] << " ";
      }
      std::cout << std::endl;
#endif
    }
  };

};
#endif

