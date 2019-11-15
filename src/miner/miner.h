#ifndef XCORTEX_MINER_H
#define XCORTEX_MINER_H

#include "net.hpp"
#include <iostream>
#include <vector>
#include <stdint.h>
#include <mutex>

class Miner{
  std::string pool_uri;
  unsigned int pool_port;
  std::string account;
  std::string worker;
  Net *net;
  mutex mut;

  std::string taskHeader, taskNonce, taskDifficulty;
  uint64_t hashs;
  uint64_t accepted;
  uint64_t rejected;

  void Login();
  void GetWork();
  void Submit(const std::string header, const uint64_t nonce);
  void Parse(const char* strJson);
  public:
    void CalculateHash();
    void PrintHashRate();
    Miner(const std::string uri, const unsigned int port, const std::string account, const std::string worker);
    void Run();
    void Stop();
};

#endif
