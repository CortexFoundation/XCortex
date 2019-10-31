#include "../src/dataset.h"
#include "../src/ops/relu.hpp"

#include <iostream>
using namespace std;
using namespace XCortex;

int main(){
    Relu relu;
    relu.init();
    relu.run();
    return 0;
}
