export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../cvm-runtime/build
export LIBRARY_PATH=$LIBRARY_PATH:../cvm-runtime/build/

CVMDIR=../cvm-runtime

cvm:
	make -C ${CVMDIR} lib -j8
	ln -sf ${CVMDIR}/build/libcvm_runtime.so ./

blake2b.o:
	g++ -c -o blake2b.o ../src/blake2b/blake2b-ref.cpp -I../src/ 
siphash.o:
	g++ -c -o siphash.o ../src/siphash/siphash.cc 
test_dense: blake2b.o siphash.o cvm
	g++ -o test_dense blake2b.o siphash.o test_dense.cpp -I${CVMDIR}/include  -lcvm_runtime --std=c++11 -pthread -fopenmp -I../src/ -I../src/ops/ -g -DDEBUG && ./test_dense	
test_xcortex: blake2b.o siphash.o cvm
	g++ -o test_xcortex test_xcortex.cpp blake2b.o siphash.o -I${CVMDIR}/include -L${CVMDIR}/build/ -lcvm_runtime --std=c++11 -pthread -fopenmp -I./ -g && ./test_xcortex

miner: blake2b.o siphash.o
	g++ ../src/miner/miner.cpp siphash.o blake2b.o -o miner -I../include/rapidjson -std=c++11 -I${CVMDIR}/include -lpthread -L${CVMDIR}/build/ -lcvm_runtime -fopenmp -g && ./miner
      
test_verify: test_verify.cpp blake2b.o siphash.o
	g++ test_verify.cpp ../src/verify.cpp siphash.o blake2b.o -o test_verify -I../include/rapidjson -std=c++11 -I${CVMDIR}/include -lpthread -lcvm_runtime && ./test_verify

clean:
	rm test_dense test_relu test_xcortex miner test_verify
