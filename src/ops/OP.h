#ifndef XCORTEX_OP_H
#define XCORTEX_OP_H

#include "../common.h"

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

    void init_inputs(Random& xcortex_random){
        std::cout << "init inputs" << std::endl;
        init_shape();

        for(int i = 0; i < num_inputs; i++){
            uint64_t size = ishape[i].Size();
            //init input data
            DLTensor *dl;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);

            DLTensor* cpu_tensor;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, kDLCPU, 0, &cpu_tensor);
            std::vector<int32_t> data = xcortex_random.generate_random_value(size);
            
           // std::cout << "input: " << i << ":" << size << std::endl;
           // for(int j = 0; j < size; j++){
           //     std::cout << data[j] << " ";
           // }
           // std::cout << std::endl;

            memcpy(cpu_tensor->data, data.data(), sizeof(int32_t) * size);
            CVMArrayCopyFromTo(cpu_tensor, dl, nullptr);
            CVMArrayFree(cpu_tensor);
            inputs[i] = *dl;
        }
    }
    void init_inputs(){
        init_shape();

        for(int i = 0; i < num_inputs; i++){
            uint64_t size = ishape[i].Size();
            //init input data
            DLTensor *dl;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);
            inputs[i] = *dl;
        }
    }

    void random_data(DataSet& data_set){
        for(int i = 0; i < num_inputs; i++){
            uint64_t size = ishape[i].Size();
            //init input data
            DLTensor* cpu_tensor;
            CVMArrayAlloc((int64_t*)ishape[i].data(), ishape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, kDLCPU, 0, &cpu_tensor);

            std::vector<int32_t> data(size);
            data_set.Read(data, size);

#ifdef DEBUG
       //     std::cout << "input: " << i << ":" << size << std::endl;
       //     for(int j = 0; j < size; j++){
       //         std::cout << data[j] << " ";
       //     }
       //     std::cout << std::endl;
#endif
            memcpy(cpu_tensor->data, data.data(), sizeof(int32_t) * size);
            CVMArrayCopyFromTo(cpu_tensor, &inputs[i], nullptr);
            CVMArrayFree(cpu_tensor);
        }
    }

    void infer_shape(){
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
        try{
          bool infer_shape_ret = finfer(attr, &ishape, &oshape);
#ifdef DEBUG
       //   if(infer_shape_ret){
       //     std::cout << "FInferShape ishape=[";
       //     for (auto& shp : ishape) std::cout << shp << ", ";
       //     std::cout << "] oshape=[";
       //     for (auto& shp : oshape) std::cout << shp << ", ";
       //     std::cout << "]\n";
       //   }
#endif
        }catch(const std::exception& e){
            std::cerr << "FInferShape error with " << e.what() << std::endl;
        }

        for(int i = 0; i < num_outputs; i++){
            DLTensor* dl;
            CVMArrayAlloc((int64_t*)oshape[i].data(), oshape[i].ndim(), dtype_code, dtype_bits, dtype_lanes, ctx, device_id, &dl);
            outputs[i] = *dl;
        }
    }

    void init(Random& xcortex_random){
      init_shape();
      init_param();
      init_inputs(xcortex_random);
      infer_shape();
    }
    void init(){
      init_shape();
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
//      for(int i = 0; i < num_outputs; i++){
//        std::cout << "output " << i << std::endl;
//        int32_t *data = (int32_t*)outputs[i].data;
//        for(int j = 0; j < oshape[i].Size(); j++){
//            std::cout << data[j] << " ";
//        }
//        std::cout << std::endl;
//      }
    }
    void run(DataSet& data_set){
      run(); 

      for(int i = 0; i < num_outputs; i++){
        const uint32_t out_size = oshape[i].Size();
        int32_t* out_data = (int32_t*)outputs[i].data;
        data_set.Write(out_data, out_size);
      }
    }

    void release(){
      cout << "release op......." << endl;
      for(size_t i = 0; i < inputs.size(); i++){
        CVMArrayFree(&inputs[i]);
      }
      for(size_t i = 0; i < outputs.size(); i++){
        CVMArrayFree(&outputs[i]);
      }
    }

    virtual ~OP(){
      release();
    };
  };

};
#endif
