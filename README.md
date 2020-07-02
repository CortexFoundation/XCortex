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

batch=8, n=8, k=8
first =0: 218 12
first =0: -108 114
first =0: -49 72
first =1: -125 152
first =0: 156 54
first =0: 61 124
first =0: 18 3
first =2: 83 94
batch=8, n=8, k=8
first =1: -123 126
first =1: -3 161
first =0: 49 63
first =1: -101 118
first =1: -170 33
first =2: -82 43
first =1: -51 118
first =2: -127 35
16

real	0m0.940s
user	0m0.954s
sys	0m0.004s

make test_xcortex 

header : 00000000000000000000000000000000000000000000000000000000000000000000000000000000
batch=8, n=8, k=8
first =0: 218 12
first =0: -108 114
first =0: -49 72
first =1: -125 152
first =0: 156 54
first =0: 61 124
first =0: 18 3
first =2: 83 94
batch=8, n=8, k=8
first =1: -123 126
first =1: -3 161
first =0: 49 63
first =1: -101 118
first =1: -170 33
first =2: -82 43
first =1: -51 118
first =2: -127 35
nonce 0 hash : 144 16 98 70 97 171 9 150 148 227 222 30 132 111 32 99 172 125 0 1 182 51 196 128 78 113 88 98 244 170 150 194 

real	0m0.956s
user	0m0.971s
sys	0m0.005s
```

# Mining
1. open the src/miner/miner.cpp
2. modify the ip and port of pool
3. make miner
4. ./miner
