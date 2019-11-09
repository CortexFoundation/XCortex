#include "../src/dataset.h"
#include "../src/ops/dense.hpp"

#include <iostream>
using namespace std;
using namespace XCortex;

int main(){
    Random xcortex_random;
    DataSet data_set(&xcortex_random);
    Dense dense;
    dense.init();
    dense.random_data(data_set);
    dense.run(data_set);
    return 0;
}
