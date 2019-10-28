#ifndef XCORTEX_DENSE_H
#define XCORTEX_DENSE_H

#include "OP.h"

namespace XCortex{

  class Dense : public OP{
    private:
      const uint32_t units = 1;
      const bool use_bias = true;
    public:
      Dense(){
        name = "dense";
        num_inputs = 3;
        num_outputs = 1;
        dims = 2;
        ishape.resize(num_inputs);
        inputs.resize(num_inputs);
        oshape.resize(num_outputs);
        outputs.resize(num_outputs);
        attr_str = "{}";
      }

      void init_param(){
        std::cout << "init param" << std::endl;
        attr.name = name;
        std::cerr << "op_type " << name << "\n";
        attr.op = cvm::Op::Get(name);
        std::cerr << "op_type =====" << name << "\n";

        attr_str = "{\"units\":\"1\", \"use_bias\":\"True\"}";

        std::istringstream is(attr_str);
        utils::JSONReader reader(&is);
        reader.Read(&attr.dict);
        if (attr.op->attr_parser) {
          attr.op->attr_parser(&attr);
        }
      }
      void init_shape(){
        //init in shape
        TShape shp(dims);
        std::vector<uint64_t> tshape(dims);
        shp[0] = 2;//xcortex_random.generate_random_value();
        shp[1] = 2;//xcortex_random.generate_random_value();
        ishape[0] = shp;
        shp[0] = 1;//xcortex_random.generate_random_value();
        shp[1] = 2;//xcortex_random.generate_random_value();
        ishape[1] = shp;
        TShape bias_shape(1);
        bias_shape[0] = 1;//xcortex_random.generate_random_value();
        ishape[2] = bias_shape;
      }
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
  };
};


#endif
