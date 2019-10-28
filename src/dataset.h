#ifndef XCORTEX_DATASET_H
#define XCORTEX_DATASET_H

#include "common.h"

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
    void Read(std::vector<int32_t>& vec, const uint32_t start, const uint32_t n){
      std::copy_n(data.begin() + start, n, vec.begin()); 
    }
    void Write(const std::vector<int32_t>& vec, const uint32_t start){
      std::copy_n(vec.begin(), vec.size(), data.begin() + start);
    }
    void Write(const int32_t* vec, const uint32_t start, const uint32_t n){
      std::copy_n(vec, n, data.begin() + start);
    }
  };

};
#endif

