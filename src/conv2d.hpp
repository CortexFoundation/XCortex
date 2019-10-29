#ifndef XCORTEX_CONV2D_H
#define XCORTEX_CONV2D_H

#include "OP.h"

namespace XCortex{
  class Conv2d : public OP{
    private:
      const uint32_t channels = OP_SHAPE;
      const uint32_t kernel_h = 3;
      const uint32_t kernel_w = 3;
    public:
      Conv2d(){
        name = "conv2d";
        num_inputs = 3;
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
        char str[1024] = "{\"channels\":\"%d\", \"kernel_size\":\"[%d, %d]\", \"strides\":\"[1, 1]\", \"padding\":\"[2, 2]\", \"dilation\":\"(1, 1)\", \"groups\":\"1\", \"layout\":\"NCHW\", \"kernel_layout\":\"OIHW\", \"use_bias\":\"True\"}";
        std::sprintf(str, str, channels, kernel_h, kernel_w);
        attr_str = str;
        //std::cout << attr_str << std::endl;
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
        shp[1] = channels;
        shp[2] = OP_SHAPE;
        shp[3] = OP_SHAPE;
        ishape[0] = shp;
        shp[2] = kernel_h;
        shp[3] = kernel_w;
        ishape[1] = shp;

        TShape bias_shape(1);
        bias_shape[0] = OP_SHAPE;//xcortex_random.generate_random_value();
        ishape[2] = bias_shape;
      }
  };
};


#endif
