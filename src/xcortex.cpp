#include "dataset.h"
#include "dense.hpp"
#include "relu.hpp"

#include <iostream>
using namespace std;
using namespace XCortex;

const int MAX_OP = 4;
const int MIN_OP = 3;

void xcortex(vector<XCortex::OP*> ops, DataSet& data_set){
  int num_ops = xcortex_random.generate_random_value(MIN_OP, MAX_OP);
  //random ops
  vector<int32_t> op_ids(num_ops);
  op_ids = xcortex_random.generate_random_value(num_ops, 0, ops.size()); 
  //execute ops
  for(int i = 0; i < num_ops; i++){
    const int op_id = op_ids[i];
    OP* op = ops[op_id];
    cout << "*****op " << i << ": " << op->name << endl;
    op->init(data_set);
    op->run(data_set);
  }
}
int main(){
//  vector<const Op*> ops = utils::Registry<Op>::List();
//  for(int i = 0; i < ops.size(); i++){
//    cout << "name: " <<ops[i]->name 
//        << ", num_inputs: " << ops[i]->num_inputs 
//        << ", num_outputs: " << ops[i]->num_outputs
//        << ", arguments: " << ops[i]->arguments.size()
//        << endl;
//  }
  DataSet data_set(1024);
  vector<XCortex::OP*> ops;
  ops.push_back(new Dense());
  ops.push_back(new Relu());
  xcortex(ops, data_set);
  return 0;
}
