#ifndef XCORTEX_RANDOM_H
#define XCORTEX_RANDOM_H

#include <vector>
#include <cstdlib>
#include <stdint.h>
#include <time.h>


namespace XCortex{

  class Random{
    uint32_t nonce;
    public:
    Random(){
      srand((unsigned)time(0));
    }
    int32_t generate_random_value(){
      ++nonce;
      return rand() % 255 - 127;
    }
    int32_t generate_random_value(const uint32_t min, const uint32_t max){
      ++nonce;
      const uint32_t size = max - min;
      return rand() % size + min;
    }
    std::vector<int32_t> generate_random_value(const uint32_t n){
      std::vector<int32_t> out(n);
      for(uint32_t i = 0; i < n; i++){
        out[i] = generate_random_value();
      }
      return out;
    }
    std::vector<int32_t> generate_random_value(const uint32_t n, const uint32_t min, const uint32_t max){
      std::vector<int32_t> out(n); 
      for(uint32_t i = 0; i < n; i++){
        out[i] = generate_random_value(min, max);
      }
      return out;
    }
  };
};
#endif

