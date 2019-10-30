#ifndef XCORTEX_MAXPOOL2D_H
#define XCORTEX_MAXPOOL2D_H

#include "OP.h"

namespace XCortex{
  class MaxPool2d: public OP{ private:
      const uint32_t pool_h = 3;
      const uint32_t pool_w = 3;
    public:
      MaxPool2d(){
        name = "max_pool2d";
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
        char str[1024] = "{\"pool_size\":\"[%d, %d]\", \"strides\":\"(1, 1)\", \"padding\":\"[0, 0]\", \"ceil_mode\":\"False\"}";
        std::sprintf(str, str, pool_h, pool_w);
        attr_str = str;
      //  std::cout << attr_str << std::endl;
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
        shp[0] = OP_SHAPE;
        shp[1] = OP_SHAPE;
        shp[2] = OP_SHAPE;
        shp[3] = OP_SHAPE;
        ishape[0] = shp;
      }
  };
};

#endif
