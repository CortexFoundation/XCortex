# How to use randomX and progpow on cortex blockchain ?

All would be changed on developing
# Getting the sources
```
git clone --recursive git@github.com:CortexFoundation/XCortex.git
```
Alternatively :
```
git clone git@github.com:CortexFoundation/XCortex.git
cd XCortex
git submodule update --init --recursive
```

# Testing
```
cd tests
make test_verify 
make test_xcortex 
```

# Mining
1. open the src/miner/miner.cpp
2. modify the ip and port of pool
3. make miner
4. ./miner
