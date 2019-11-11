#include "../src/verify.h"
#include <iostream>
#include <stdint.h>
#include <unistd.h>
using namespace std;

#define HEADERLEN 80
int main(int argc, char **argv){
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

  int8_t hash[] = {1,2,23,3,4,4};
  bool ret = verify(nonce, header, hash);
  return 0;
}
