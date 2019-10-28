#ifndef XCORTEX_COMMON_H
#define XCORTEX_COMMON_H

#include <cvm/c_api.h>
#include <cvm/model.h>
#include <numeric>
#include <thread>
#include <omp.h>
#include <utils/registry.h>
#include <cvm/runtime/registry.h>
#include <cvm/op.h>
#include <cvm/op_attr_types.h>
#include <cvm/runtime/ndarray.h>
#include <cvm/runtime/packed_func.h>
#include <cvm/runtime/registry.h>
#include <cvm/runtime/serializer.h>
#include <cvm/node.h>
#include <cvm/runtime/c_runtime_api.h>

#include <iostream>
#include <string>
#include <vector>

#include "random.h"
#include "dataset.h"
static XCortex::Random xcortex_random;
using namespace cvm;
using namespace cvm::runtime;
#endif
