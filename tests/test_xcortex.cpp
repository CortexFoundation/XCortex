#include "../src/dataset.h"
#include "../src/dense.hpp"
#include "../src/relu.hpp"
#include "../src/conv2d.hpp"
#include "../src/broadcast.hpp"
#include "../src/reduce.hpp"

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
  ops.push_back(new Broadcast("add"));
  ops.push_back(new Broadcast("sub"));
  ops.push_back(new Broadcast("mul"));
  ops.push_back(new Broadcast("max"));
  ops.push_back(new Reduce("max"));
  ops.push_back(new Reduce("sum"));

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
