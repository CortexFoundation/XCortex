#include "xcortex.hpp"
#include "verify.h"
#include <iostream>
using namespace std;
using namespace XCortex;

bool verify(uint32_t nonce, vector<uint8_t> header, const int8_t *hash){
  XCortex::XCortex xcortex; 
  xcortex.set_header_nonce(header, nonce);
  uint8_t hash_result[32];
  xcortex.run(hash_result, sizeof(hash_result));
  return memcmp(hash_result, hash, 32) == 0;
} 

