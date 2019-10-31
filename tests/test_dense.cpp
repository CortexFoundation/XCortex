#include "../src/dataset.h"
#include "../src/ops/dense.hpp"

#include <iostream>
using namespace std;
using namespace XCortex;

int main(){
    DataSet data_set(DATA_SET_SIZE);
    Dense dense;
    dense.init(data_set);
    dense.run(data_set);
    return 0;
}
