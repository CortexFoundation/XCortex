CVMDIR=../cvm-runtime

blake2b.o:
	g++ -fPIC -c -o blake2b.o src/blake2b/blake2b-ref.cpp -Isrc/ 
siphash.o:
	g++ -fPIC -c -o siphash.o src/siphash/siphash.cc 
xcortex.o:
	g++ -fPIC -c -o xcortex.o src/xcortex.hpp -I${CVMDIR}/include -std=c++11
verify.o:
	g++ -fPIC src/verify.cpp -c -o $@ -std=c++11 -I${CVMDIR}/include
	
libverify.so: blake2b.o siphash.o xcortex.o verify.o
	ar r $@ $^

miner: blake2b.o siphash.o
	make -C ./cvm-runtime/ cpu
	g++ src/miner/miner.cpp siphash.o blake2b.o -o miner -Iinclude/rapidjson -std=c++11 -I${CVMDIR}/include -lpthread -L${CVMDIR}/build/cpu/ -lcvm_runtime_cpu -fopenmp

clean:
	rm -f blake2b.o siphash.o xcortex.o verify.o libverify.so miner
