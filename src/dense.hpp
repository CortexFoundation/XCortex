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
        attr.name = name;
        attr.op = cvm::Op::Get(name);
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
        //shp[1] = 2;//xcortex_random.generate_random_value();
        ishape[1] = shp;
        TShape bias_shape(1);
        bias_shape[0] = 1;//xcortex_random.generate_random_value();
        ishape[2] = bias_shape;
      }
  };
};


#endif
