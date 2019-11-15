#ifndef XCORTEX_STRIDED_SLICE_H
#define XCORTEX_STRIDED_SLICE_H

#include "OP.h"

namespace XCortex{
  class StridedSlice: public OP{
    public:
      StridedSlice(){
        this->name = "strided_slice";
        num_inputs = 1;
        num_outputs = 1;
        dims = 4;
        ishape.resize(num_inputs);
        inputs.resize(num_inputs);
        oshape.resize(num_outputs);
        outputs.resize(num_outputs);
        attr_str = "{\"begin\":\"[0, 0]\", \"end\":\"[6, 6]\", \"stride\":\"[1, 1]\"}";
      }

      void init_param(){
        attr.name = name;
        attr.op = cvm::Op::Get(name);
        assert(attr.op != nullptr);

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
  REGISTEROP(StridedSlice);
};


#endif
