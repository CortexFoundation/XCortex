#include "common.h"

#include <iostream>
using namespace std;
using namespace XCortex;

namespace XCortex{
  class XCortex{
    siphash_keys sipkeys;
    public:
      XCortex(){
      }
      void setheader(const char *header, const u32 headerlen, siphash_keys *keys) {
        char hdrkey[32];
        // SHA256((unsigned char *)header, headerlen, (unsigned char *)hdrkey);
        //  printf("call blake2b %d %d %d %d\n", hdrkey, sizeof(hdrkey), header, headerlen);
        blake2b((void *)hdrkey, sizeof(hdrkey), (const void *)header, headerlen, 0, 0);
        //  assert(0);
#ifdef SIPHASH_COMPAT
        u64 *k = (u64 *)hdrkey;
        u64 k0 = k[0];
        u64 k1 = k[1];
        printf("k0 k1 %lx %lx\n", k0, k1);
        k[0] = k0 ^ 0x736f6d6570736575ULL;
        k[1] = k1 ^ 0x646f72616e646f6dULL;
        k[2] = k0 ^ 0x6c7967656e657261ULL;
        k[3] = k1 ^ 0x7465646279746573ULL;
#endif
        setkeys(keys, hdrkey);
      }
      void set_header_nonce(std::vector<uint8_t> header, uint64_t nonce){
        uint64_t littleEndianNonce = htole64(nonce);
        char headerBuf[40];
        memcpy(headerBuf, header.data(), 32);
        memcpy(headerBuf + 32, static_cast<uint64_t*>(&littleEndianNonce), sizeof(nonce));
        setheader(headerBuf, 40, &sipkeys);
        xcortex_random.set_keys(sipkeys);
      }
      void run(vector<OP*> ops, DataSet& data_set){
        int num_ops = NUM_OPS;//xcortex_random.generate_random_value(MIN_OP, MAX_OP);
        //random ops
        vector<int32_t> op_ids(num_ops);
        op_ids = xcortex_random.generate_random_value(num_ops, 0, ops.size()); 
        //execute ops
        for(int i = 0; i < num_ops; i++){
          const int op_id = op_ids[i];
          OP* op = ops[op_id];
#ifdef DEBUG
          cout << "*****op " << i << ": " << op->name << endl;
#endif
          op->init(data_set);
          op->run(data_set);
        }
      }
  };

};
