
cvm:
	make -C ../cvm-runtime cpu -j8
	cp ../cvm-runtime/build/cpu/libcvm_runtime_cpu.so ./
test_dense: cvm
	g++ -o test_dense ../src/dense.hpp test_dense.cpp -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g -DDEBUG && ./test_dense	
test_relu: cvm
	g++ -o test_relu ../src/relu.hpp test_relu.cpp -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g -DDEBUG && ./test_relu	
test_xcortex: cvm
	g++ -o test_xcortex test_xcortex.cpp ../src/blake2b-ref.cpp ../src/siphash.cc -I../cvm-runtime/include -L../cvm-runtime/build/cpu/ -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g && ./test_xcortex
      

clean:
	rm test_dense test_relu test_xcortex
