#ifndef XCORTEX_DATASET_H
#define XCORTEX_DATASET_H

#include "common.h"

#define XCORTEX_BITS 10
#define DATA_SET_SIZE (1 << XCORTEX_BITS) 
#define DATA_SET_MASK (DATA_SET_SIZE - 1)

namespace XCortex{
  struct Random;
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
        //vec[i] < 0 : TODO yangyang
        uint32_t index = (vec[i] < 0 ? -vec[i] : vec[i]) & DATA_SET_MASK;
        data[index] = vec[i];
      }
    }
  };

};
#endif

