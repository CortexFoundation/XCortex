#ifndef XCORTEX_RESHAPE_H
#define XCORTEX_RESHAPE_H

#include "OP.h"

namespace XCortex{
  class Reshape: public OP{
    public:
      Reshape(){
        this->name = "reshape";
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
        uint32_t size = ishape[0].Size();
        char str[1024];
        sprintf(str,"{\"shape\":\"[1, 1, %d, 1]\"}", size);
        attr_str = str;

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
  REGISTEROP(Reshape);
};


#endif
