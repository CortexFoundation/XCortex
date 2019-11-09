#ifndef XCORTEX_MINER_H
#define XCORTEX_MINER_H

#include "net.hpp"
#include <iostream>
#include <vector>

class Miner{
  std::string pool_uri;
  unsigned int pool_port;
  std::string account;
  std::string worker;
  Net *net;

  std::string taskHeader, taskNonce, taskDifficulty;

  void Login();
  void GetWork();
  void Submit(const std::vector<char>& hash);
  void Parse(const char* strJson);
  public:
    void CalculateHash();
    Miner(const std::string uri, const unsigned int port, const std::string account, const std::string worker);
    void Run();
    void Stop();
};

#endif
