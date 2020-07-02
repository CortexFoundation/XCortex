#include "../src/verify.h"
#include <iostream>
#include <stdint.h>
#include <vector>
#include <unistd.h>
using namespace std;

#define HEADERLEN 80
int main(int argc, char **argv){
  //header
  vector<uint8_t> header(HEADERLEN, 0);
  uint64_t nonce = 0;
  uint32_t range = 1;
  int c;
  while ((c = getopt (argc, argv, "ah:m:n:r:st:x:")) != -1) {
    switch (c) {
      case 'n':
        nonce = atoi(optarg);
        break;
    }
  }

  int8_t difficulty[64] = "0x800000000000";
  int ret = Verify(nonce, (uint8_t*)header.data(), (uint8_t*)difficulty);
  cout << ret << endl;
  return 0;
}
