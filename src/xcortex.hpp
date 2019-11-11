#include "common.h"
#include "../src/ops/dense.hpp"
#include "../src/ops/relu.hpp"
#include "../src/ops/conv2d.hpp"
#include "../src/ops/maxpool2d.hpp"
#include "../src/ops/broadcast.hpp"
#include "../src/ops/elemwise.hpp"
#include "../src/ops/reduce.hpp"
#include "../src/ops/clip.hpp"
#include "../src/ops/flatten.hpp"
#include "../src/ops/reshape.hpp"
#include "../src/ops/cvm_clip.hpp"
#include "../src/ops/cvm_shift.hpp"
#include "../src/ops/cvm_precision.hpp"
#include "../src/ops/abs.hpp"
#include "../src/ops/concatenate.hpp"
#include "../src/ops/repeat.hpp"
#include "../src/ops/negative.hpp"
#include "../src/ops/tile.hpp"
#include "../src/ops/expand_dims.hpp"
#include "../src/ops/squeeze.hpp"
#include "../src/ops/transpose.hpp"
#include "../src/ops/strided_slice.hpp"
#include "../src/ops/slice_like.hpp"
#include "../src/ops/take.hpp"
#include "../src/ops/cvm_lut.hpp"
#include "../src/ops/upsampling.hpp"
#include "../src/ops/get_valid_count.hpp"
#include "../src/ops/non_max_suppression.hpp"

#include "../src/siphash/siphash.h" 
#include "../src/blake2b/blake2.h"

using namespace std;

namespace XCortex{
  class XCortex{
    siphash_keys sipkeys;
    Random xcortex_random;
    std::vector<OP*> ops;
    public:
      XCortex(){
        vector<void*> objList = Reflector::Instance()->ObjectList(); 
        ops.resize(objList.size());
        for(int i = 0; i < objList.size(); i++){
          ops[i] = (OP*)objList[i];
          ops[i]->init();
        }
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
        k[1] = k1 ^ 0x646f72616e646f6dULL; k[2] = k0 ^ 0x6c7967656e657261ULL;
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

      void blake2b_hash(const void* data, uint32_t data_size, uint8_t* result, size_t result_size){
        blake2b((void*)result, result_size, data, data_size, 0, 0);
        uint64_t hash_key[4];
        //for(int i = 0; i < 4; i++){
        //  hash_key[i] = htole64(((uint64_t*)result)[i]);
        //  cout << hash_key[i] << endl;
        //}
      }

      void run(uint8_t* result, size_t result_size){
        xcortex_random.reset();
        DataSet data_set(&xcortex_random);

        int num_ops = NUM_OPS;//xcortex_random.generate_random_value(MIN_OP, MAX_OP);
        //random ops
        vector<int32_t> op_ids(num_ops);
        op_ids = xcortex_random.generate_random_value(num_ops, 0, ops.size()); 
        //execute ops
        for(int i = 0; i < num_ops; i++){
          const int op_id = op_ids[i];
          OP* op = ops[op_id];
          std::cout << "*****op " << i << ": " << op->name << std::endl;
          op->random_data(data_set);
          op->run(data_set);
        }
        
        blake2b_hash(data_set.data.data(), data_set.size, result, result_size);
      }
  };

};
