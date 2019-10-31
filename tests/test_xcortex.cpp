#include "../src/dataset.h"
#include "../src/dense.hpp"
#include "../src/relu.hpp"
#include "../src/conv2d.hpp"
#include "../src/maxpool2d.hpp"
#include "../src/broadcast.hpp"
#include "../src/elemwise.hpp"
#include "../src/reduce.hpp"
#include "../src/clip.hpp"
#include "../src/flatten.hpp"
#include "../src/reshape.hpp"
#include "../src/cvm_clip.hpp"
#include "../src/cvm_shift.hpp"
#include "../src/cvm_precision.hpp"
#include "../src/abs.hpp"
#include "../src/concatenate.hpp"
#include "../src/repeat.hpp"
#include "../src/negative.hpp"
#include "../src/tile.hpp"
#include "../src/expand_dims.hpp"
#include "../src/squeeze.hpp"
#include "../src/transpose.hpp"
#include "../src/strided_slice.hpp"
#include "../src/slice_like.hpp"
#include "../src/take.hpp"
#include "../src/cvm_lut.hpp"
#include "../src/upsampling.hpp"
#include "../src/get_valid_count.hpp"
#include "../src/non_max_suppression.hpp"

#include "../src/siphash.h" 
#include "../src/blake2.h"
#include <unistd.h>
#include "../src/xcortex.hpp"

void blake2b_hash(const void* data, uint32_t data_size, char* result, size_t result_size){
  blake2b((void*)result, result_size, data, data_size, 0, 0);
  uint64_t hash_key[4];
  for(int i = 0; i < 4; i++){
    hash_key[i] = htole64(((uint64_t*)result)[i]);
    cout << hash_key[i] << endl;
  }
}

#define HEADERLEN 80
int main(int argc, char **argv){
//  vector<const Op*> ops = utils::Registry<Op>::List();
//  for(int i = 0; i < ops.size(); i++){
//    cout << "name: " <<ops[i]->name 
//        << ", num_inputs: " << ops[i]->num_inputs 
//        << ", num_outputs: " << ops[i]->num_outputs
//        << ", arguments: " << ops[i]->arguments.size()
//        << endl;
//  }
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

  vector<XCortex::OP*> ops;
  ops.push_back(new Dense());
  ops.push_back(new Relu());
  ops.push_back(new Conv2d());
  ops.push_back(new MaxPool2d());
  ops.push_back(new Broadcast("add"));
  ops.push_back(new Broadcast("sub"));
  ops.push_back(new Broadcast("mul"));
  ops.push_back(new Broadcast("max"));
  ops.push_back(new Reduce("max"));
  ops.push_back(new Reduce("sum"));
  ops.push_back(new Elemwise("add"));
  ops.push_back(new Elemwise("sub"));
  ops.push_back(new Clip());
  ops.push_back(new Flatten());
  ops.push_back(new Reshape());
  ops.push_back(new CVMClip());
  ops.push_back(new CVMShift("right"));
  //ops.push_back(new CVMShift("left"));
  ops.push_back(new Abs());
  ops.push_back(new CVMPrecision());
  ops.push_back(new Concatenate());
  ops.push_back(new Repeat());
  ops.push_back(new Negative());
  ops.push_back(new Tile());
  ops.push_back(new ExpandDims());
  ops.push_back(new Squeeze());
  ops.push_back(new Transpose());
  ops.push_back(new StridedSlice());
  ops.push_back(new SliceLike());
  ops.push_back(new Take());
  ops.push_back(new CVMLut());
  ops.push_back(new UpSampling());
  ops.push_back(new GetValidCounts());
  //ops.push_back(new NonMaximumSuppression());

  for(int i = 0; i < range; i++){
    XCortex::XCortex xcortex; 
    xcortex.set_header_nonce(header, nonce + i);

    xcortex_random.reset();

    DataSet data_set(DATA_SET_SIZE);
    //data_set.print();
    cout << "first hash :" << endl;
    char hash_result[32];
    blake2b_hash(data_set.data.data(), data_set.size, hash_result, sizeof(hash_result));

    xcortex.run(ops, data_set);

    //data_set.print();
    cout << "last hash:" << endl;
    blake2b_hash(data_set.data.data(), data_set.size, hash_result, sizeof(hash_result));
    cout << endl;
  }
  return 0;
}
