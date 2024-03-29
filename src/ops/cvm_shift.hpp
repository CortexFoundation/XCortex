#ifndef XCORTEX_CVM_SHIFT_H
#define XCORTEX_CVM_SHIFT_H

#include "OP.h"

namespace XCortex{
  class CVMShift: public OP{
    public:
      CVMShift(std::string name){
        this->name = "cvm_" + name + "_shift";
        num_inputs = 1;
        num_outputs = 1;
        dims = 4;
        ishape.resize(num_inputs);
        inputs.resize(num_inputs);
        oshape.resize(num_outputs);
        outputs.resize(num_outputs);
        attr_str = "{}";
      }

      void init_param(){
        attr.name = name;
        attr.op = cvm::Op::Get(name);
        assert(attr.op != nullptr);
        attr_str = "{\"precision\":\"8\", \"shift_bit\":\"1\"}";

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
        for(int i = 0; i < dims; i++){
          shp[i] = OP_SHAPE;
        }
        for(int i = 0; i < num_inputs; i++){
          ishape[i] = shp;
        }
      }
  };
//  REGISTER_OP_BY_NAME(CVMShift, "left");
  REGISTEROP_BY_NAME(CVMShift, right);
};

#endif
