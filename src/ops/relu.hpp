#ifndef XCORTEX_RELU_H
#define XCORTEX_RELU_H

#include "OP.h"

namespace XCortex{

  class Relu : public OP{
    public:
      Relu(){
        name = "relu";
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

        attr_str = "{}";//"{\"units\":\"1\", \"use_bias\":\"True\"}";

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
        shp[0] = OP_SHAPE;//xcortex_random.generate_random_value();
        shp[1] = OP_SHAPE;//xcortex_random.generate_random_value();
        shp[2] = OP_SHAPE;//xcortex_random.generate_random_value();
        shp[3] = OP_SHAPE;//xcortex_random.generate_random_value();
        ishape[0] = shp;
      }

      void init_inputs(OP* preOp){
        if(preOp->num_outputs > num_inputs){
            for(int i = 0; i < num_inputs; i++){

            }
        }
      }

  };
  REGISTEROP(Relu);
};


#endif
