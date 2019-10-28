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
    std::vector<int32_t> generate_random_value(uint32_t n){
      std::vector<int32_t> out(n);
      for(uint32_t i = 0; i < n; i++){
        out[i] = generate_random_value();
      }
      return out;
    }
  };
};
#endif

