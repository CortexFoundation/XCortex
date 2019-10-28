#ifndef XCORTEX_OP_H
#define XCORTEX_OP_H

#include "common.h"

namespace XCortex{

  int dtype_code{kDLInt};
  int dtype_bits{32};
  int dtype_lanes{1};
  int ctx = kDLCPU;
  int device_id = 0;

  struct OpArgs {
    std::vector<DLTensor> args;
    std::vector<CVMValue> arg_values;
    std::vector<int> arg_tcodes;
    std::vector<int64_t> shape_data;
  };

  struct OP{
    string name;
    uint32_t num_inputs;
    uint32_t num_outputs;
    uint32_t dims;
    std::vector<DLTensor> inputs;
    std::vector<DLTensor> outputs;
    std::vector<TShape> ishape;
    std::vector<TShape> oshape;
    std::string attr_str;
    NodeAttrs attr;
    virtual void init_param() = 0;
    virtual void init_inputs() = 0;
    virtual void infer_shape() = 0;

    void init(){
      init_param();
      init_inputs();
      infer_shape();
    }

    void run(){
      std::vector<DLTensor> args(num_inputs + num_outputs);
      for(int i = 0; i < num_inputs; i++){
        args[i] = inputs[i];
      }
      for(int i = 0; i < num_outputs; i++){
        args[num_inputs + i] = outputs[i];
      }

      std::shared_ptr<OpArgs> arg_ptr = std::make_shared<OpArgs>();
      // setup address.
      arg_ptr->args = args;
      for (size_t i = 0; i < arg_ptr->args.size(); ++i) {
        CVMValue v;
        DLTensor* t = &(arg_ptr->args[i]);
        v.v_handle = t;
        arg_ptr->arg_values.push_back(v);
        arg_ptr->arg_tcodes.push_back(kArrayHandle);
      }
      CVMValue t_attr;
      t_attr.v_handle = (void*)&attr;
      arg_ptr->arg_values.push_back(t_attr);
      arg_ptr->arg_tcodes.push_back(kHandle);

      auto op_name = name;
      int device_type = static_cast<int>(ctx);
      std::string module_name = "cvm.runtime.cvm";
      if (device_type == kDLGPU) module_name += "_cuda";
      module_name += ".";
      auto func = cvm::runtime::Registry::Get(module_name + op_name);
      VERIFY(func != nullptr) << "function undefined " << module_name + op_name;
      auto op = [arg_ptr, op_name, func](){
        CVMRetValue rv;
        CVMArgs targs(
            arg_ptr->arg_values.data(),
            arg_ptr->arg_tcodes.data(),
            static_cast<int>(arg_ptr->arg_values.size())
            );
        func->CallPacked(targs, &rv);
      };
      op();
#ifdef DEBUG
      for(int i = 0; i < num_outputs; i++){
        std::cout << "output " << i << std::endl;
        int32_t *data = (int32_t*)outputs[i].data;
        for(int j = 0; j < oshape[i].Size(); j++){
            std::cout << data[j] << " ";
        }
        std::cout << std::endl;
      }
#endif
    }
  };

};
#endif
