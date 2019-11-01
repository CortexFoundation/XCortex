#ifndef XCORTEX_RANDOM_H
#define XCORTEX_RANDOM_H

#include <vector>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include "siphash/siphash.h"


namespace XCortex{

  class Random{
    uint32_t nonce;
    siphash_keys sipkeys;
    public:
    Random(){
      srand((unsigned)time(0));
      nonce = 0;
    }
    Random(const siphash_keys& sipkeys){
      this->sipkeys.k0 = sipkeys.k0;
      this->sipkeys.k1 = sipkeys.k1;
      this->sipkeys.k2 = sipkeys.k2;
      this->sipkeys.k3 = sipkeys.k3;
    }
    void reset(){
      nonce = 0;
    }
    void set_keys(const siphash_keys& sipkeys){
      this->sipkeys.k0 = sipkeys.k0;
      this->sipkeys.k1 = sipkeys.k1;
      this->sipkeys.k2 = sipkeys.k2;
      this->sipkeys.k3 = sipkeys.k3;
    }
    int32_t generate_random_value(){
      uint64_t r = siphash24(&sipkeys, nonce++);
      return r % 255 - 127;
      //return rand() % 255 - 127;
    }
    int32_t generate_random_value(const uint32_t min, const uint32_t max){
      //++nonce;
      const uint32_t size = max - min;
      //return rand() % size + min;
      uint64_t r = siphash24(&sipkeys, nonce++);
      return r % size + min;
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

