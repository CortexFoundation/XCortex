#include <unistd.h>
#include "../src/xcortex.hpp"
#include <iostream>
using namespace std;
using namespace XCortex;


#define HEADERLEN 80
int main(int argc, char **argv){
  vector<uint8_t> header(HEADERLEN, 0);
  uint64_t nonce = 0;
  uint32_t range = 1;
  int c;
  while ((c = getopt (argc, argv, "ah:m:n:r:st:x:")) != -1) {
    switch (c) {
      case 'r':
        range = atoi(optarg);
        break;
      case 'n':
        nonce = atoi(optarg);
        break;
    }
  }


  for(int i = 0; i < range; i++){
    XCortex::XCortex xcortex; 
    Random xcortex_random;
    xcortex_random.set_keys(xcortex.set_header_nonce(header, nonce+i));
    uint8_t hash_result[32];
    xcortex.run(hash_result, sizeof(hash_result), xcortex_random);
    for(int i = 0; i < 32; i++){
      cout << (uint32_t)hash_result[i] << " ";
    }
    cout << endl;
  }
  return 0;
}
