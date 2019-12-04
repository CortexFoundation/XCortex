#include "xcortex.hpp"
#include "verify.h"
#include "hex.hpp"
#include <string>
#include <iostream>
using namespace std;
using namespace XCortex;


int Verify(const uint64_t nonce, const uint8_t* header, const uint8_t *difficulty){
#ifdef DEBUG
  cout << "verify : nonce = " << nonce << ", difficulty=" << difficulty << endl; 
#endif
  static XCortex::XCortex xcortex; 
  vector<uint8_t> h(header, header+32);
  xcortex.set_header_nonce(h, nonce);
  uint8_t hash_result[32];
  xcortex.run(hash_result, sizeof(hash_result));
  const uint8_t *p = difficulty[1] == 'x' ? difficulty + 2 : difficulty;
  string strDiff((char*)(p));
  vector<uint8_t> diff = XCortex::hex.DecodeString(strDiff);

#ifdef DEBUG
  cout << "hash result: "; 
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)hash_result[i] << " ";
  }
  cout << endl;
  cout << "difficulty: ";
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)diff[i] << " ";
  }
  cout << endl;
#endif
  int ret = memcmp(hash_result, diff.data(), sizeof(hash_result));
  return ret;
} 

int Verify_remote(const uint64_t nonce, const uint8_t* header, const uint8_t* difficulty, const uint8_t *shareTarget, const uint8_t *blockTarget, int32_t*ret){
#ifdef DEBUG
  cout << "verify : nonce = " << nonce << ", shareTarget=" << shareTarget << ",blockTarget=" << blockTarget << endl; 
#endif
  static XCortex::XCortex xcortex; 
  vector<uint8_t> h(header, header+32);
  xcortex.set_header_nonce(h, nonce);
  uint8_t hash_result[32];
  xcortex.run(hash_result, sizeof(hash_result));
  
  const uint8_t *p0 = difficulty[1] == 'x' ? difficulty + 2 : difficulty;
  const uint8_t *p1 = shareTarget[1] == 'x' ? shareTarget + 2 : shareTarget;
  const uint8_t *p2 = blockTarget[1] == 'x' ? blockTarget + 2 : blockTarget;
  string strDiff0((char*)(p0));
  string strDiff1((char*)(p1));
  string strDiff2((char*)(p2));
  vector<uint8_t> diff0 = XCortex::hex.DecodeString(strDiff0);
  vector<uint8_t> diff1 = XCortex::hex.DecodeString(strDiff1);
  vector<uint8_t> diff2 = XCortex::hex.DecodeString(strDiff2);

#ifdef DEBUG
  cout << "hash result: "; 
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)hash_result[i] << " ";
  }
  cout << endl;
  cout << "difficulty: ";
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)diff0[i] << " ";
  }
  cout << endl;
  cout << "shareTarget: ";
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)diff1[i] << " ";
  }
  cout << endl;
  cout << "blockTarget: ";
  for(int i = 0; i < 32; i++){
    cout << (uint32_t)diff2[i] << " ";
  }
  cout << endl;
#endif

  int ret0 = memcmp(hash_result, diff0.data(), sizeof(hash_result));
  int ret1 = memcmp(hash_result, diff1.data(), sizeof(hash_result));
  int ret2 = memcmp(hash_result, diff2.data(), sizeof(hash_result));

  ret[0] = ret0;
  ret[1] = ret1;
  ret[2] = ret2;
  return ((ret0 < 0) && (ret1 < 0) && (ret2 < 0));
}
