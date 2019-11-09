export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../cvm-runtime/build/cpu
export LIBRARY_PATH=$LIBRARY_PATH:../cvm-runtime/build/cpu

cvm:
	make -C ../cvm-runtime -j8
#	cp ../cvm-runtime/build/cpu/libcvm_runtime_cpu.so ./

blake2b.o:
	g++ -c -o blake2b.o ../src/blake2b/blake2b-ref.cpp -I../src/ 
siphash.o:
	g++ -c -o siphash.o ../src/siphash/siphash.cc 
test_dense: blake2b.o siphash.o cvm
	g++ -o test_dense blake2b.o siphash.o test_dense.cpp -I../cvm-runtime/include  -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I../src/ -I../src/ops/ -g -DDEBUG && ./test_dense	
test_xcortex: blake2b.o siphash.o cvm
	g++ -o test_xcortex test_xcortex.cpp blake2b.o siphash.o -I../cvm-runtime/include -lcvm_runtime_cpu --std=c++11 -pthread -fopenmp -I./ -g && ./test_xcortex

miner: blake2b.o siphash.o
	g++ ../src/miner/miner.cpp siphash.o blake2b.o -o miner -I../include/rapidjson -std=c++11 -I../cvm-runtime/include -lpthread -lcvm_runtime_cpu && ./miner
      

clean:
	rm test_dense test_relu test_xcortex miner
