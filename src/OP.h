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

    virtual void init_shape() = 0;

    void init_inputs(){
        std::cout << "init inputs" << std::endl;
        init_shape();

        for(int i = 0; i < num_inputs; i++){
            uint64_t size = ishape[i].Size();
            std::cout << "input: " << i << ":" << size << std::endl;
            //init input data
            DLTensor *dl;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);

            DLTensor* cpu_tensor;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, kDLCPU, 0, &cpu_tensor);
            std::vector<int32_t> data = xcortex_random.generate_random_value(size);
#ifdef DEBUG
            for(int j = 0; j < size; j++){
                std::cout << data[j] << " ";
            }
            std::cout << std::endl;
#endif
            memcpy(cpu_tensor->data, data.data(), sizeof(int32_t) * size);
            CVMArrayCopyFromTo(cpu_tensor, dl, nullptr);
            CVMArrayFree(cpu_tensor);
            inputs[i] = *dl;
        }
    }
    void init_inputs(DataSet& data_set){
        std::cout << "init inputs" << std::endl;
        init_shape();

        for(int i = 0; i < num_inputs; i++){
            uint64_t size = ishape[i].Size();
            std::cout << "input: " << i << ":" << size << std::endl;
            //init input data
            DLTensor *dl;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);

            DLTensor* cpu_tensor;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, kDLCPU, 0, &cpu_tensor);

            //std::vector<int32_t> data = xcortex_random.generate_random_value(size);
            const int32_t data_set_index = xcortex_random.generate_random_value(0, size); 
            std::vector<int32_t> data(size);
            //std::copy_n(data_set.begin() + data_set_index, size, data.begin());
            data_set.Read(data, data_set_index, size);

#ifdef DEBUG
            for(int j = 0; j < size; j++){
                std::cout << data[j] << " ";
            }
            std::cout << std::endl;
#endif
            memcpy(cpu_tensor->data, data.data(), sizeof(int32_t) * size);
            CVMArrayCopyFromTo(cpu_tensor, dl, nullptr);
            CVMArrayFree(cpu_tensor);
            inputs[i] = *dl;
        }
    }

    void infer_shape(){
        std::cout << "infer shape" << std::endl;
        static auto& finfer_shape =
            Op::GetAttr<cvm::FInferNodeEntryAttr<TShape> >("FInferShape");
        const cvm::Op *op = cvm::Op::Get(name);
        auto finfer = finfer_shape.get(op, nullptr);
        if (finfer == nullptr) {
            std::cout << "operator " << name
                << "has not registered FInferShape";
            return;
        }
        assert(finfer != nullptr);
        bool infer_shape_ret = finfer(attr, &ishape, &oshape);
        try{
            if(infer_shape_ret){
                std::cout << "FInferShape ishape=[";
                for (auto& shp : ishape) std::cout << shp << ", ";
                std::cout << "] oshape=[";
                for (auto& shp : oshape) std::cout << shp << ", ";
                std::cout << "]\n";
            }
        }catch(const std::exception& e){
            std::cerr << "FInferShape error with " << e.what() << std::endl;
        }

        for(int i = 0; i < num_outputs; i++){
            DLTensor* dl;
            CVMArrayAlloc((int64_t*)oshape[i].data(), oshape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);
            outputs[i] = *dl;
        }
    }

    void init(){
      init_param();
      init_shape();
      init_inputs();
      infer_shape();
    }
    void init(DataSet& data_set){
      init_param();
      init_shape();
      init_inputs(data_set);
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
    void run(DataSet& data_set){
      run(); 

      for(int i = 0; i < num_outputs; i++){
        const uint32_t out_size = oshape[i].Size();
        const int32_t data_set_index = xcortex_random.generate_random_value(0, data_set.size - out_size);
        int32_t* out_data = (int32_t*)outputs[i].data;
        data_set.Write(out_data, data_set_index, out_size);
        //std::copy_n(data, out_size, data_set.begin() + data_set_index);
      }
    }

    virtual ~OP(){};
  };

};
#endif