#ifndef XCORTEX_NON_MAX_SUPPRESSION_H
#define XCORTEX_NON_MAX_SUPPRESSION_H

#include "OP.h"

namespace XCortex{
  class NonMaximumSuppression: public OP{
    public:
      NonMaximumSuppression(){
        this->name = "non_max_suppression";
        num_inputs = 2;
        num_outputs = 1;
        dims = 3;
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
        shp[2] = 6;
        ishape[0] = shp;

        TShape valid_count_shp(1);
        valid_count_shp[0] = OP_SHAPE;
        ishape[1] = valid_count_shp;
      }
  };
};


#endif

