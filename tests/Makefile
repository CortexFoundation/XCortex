test_dense:
	g++ -o test_dense ../src/dense.hpp test_dense.cpp -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g -DDEBUG && ./test_dense	
test_relu:
	g++ -o test_relu ../src/relu.hpp test_relu.cpp -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g -DDEBUG && ./test_relu	
xcortex:
	g++ -o xcortex ../src/xcortex.cpp -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g -DDEBUG && ./xcortex
      

clean:
	rm test_dense
