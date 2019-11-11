#include "net.hpp"
#include "json.hpp"
#include "../xcortex.hpp"
#include "../hex.hpp"
#include "miner.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
using namespace XCortex;

void Miner::Login(){
  char login[1024];
  sprintf(login, "{\"id\":72,\"jsonrpc\":\"2.0\",\"method\":\"ctxc_submitLogin\",\"params\":[\"%s\"],\"worker\":\"%s\"}\n", account.c_str(), worker.c_str());
  cout << "login : " << login << endl;
  net->Send(login);
}
void Miner::GetWork(){
  char getwork[] = "{\"id\":100,\"jsonrpc\":\"2.0\",\"method\":\"ctxc_getWork\",\"params\":[\"\"]}\n"; cout << "get work: " << getwork << endl;
  net->Send(getwork);
}

void Miner::Submit(const std::string header, const uint64_t nonce){
  Hex hex;
  vector<uint32_t> sol(32, 0);
  std::string solStr = "0x" + hex.Uint32ArrayToHexString(sol);
  std::string nonceStr = "0x" + hex.Uint64ToHexString(nonce);

  char submit[1024];
  sprintf(submit, "{\"id\":73,\"jsonrpc\":\"2.0\", \"method\":\"ctxc_submitWork\",\"params\":[\"%s\",\"%s\",\"%s\"],\"worker\":\"%s\"}\n", nonceStr.c_str(), header.c_str(), solStr.c_str(), worker.c_str());
  mut.lock();
  net->Send(submit);
  mut.unlock();
  cout << "submit: " << submit << endl; 
}

void Miner::Parse(const char* strJson){
  Json json(strJson);
  if(json.IsJson()){
    int id = json.GetInt("id");
    bool isAccepted;
    vector<string> result;
    switch(id){
      case 72:
        cout << "login success." << endl;
        GetWork();
        break;
      case 73:
        if(!json.IsNull("error")){
          cout << "hash error: " << strJson << endl;
        }else{
          isAccepted = json.GetBool("result");
          accepted = isAccepted ? accepted + 1 : accepted;
          rejected = isAccepted ? rejected : rejected + 1;
          cout << (isAccepted ? "share accepted" : "share reject") << endl;
          cout << "shared accepted=" << accepted << ", rejected=" << rejected << endl;
        }
        break;
      case 0:
      case 100:
        result = json.GetListString("result");
        if(result.size() >= 3){
          taskHeader = result[0];
          taskNonce = result[1];
          taskDifficulty = result[2];
          cout << "taskHeader = " << taskHeader << ", taskNonce = " << taskNonce << ", taskDifficulty = " << taskDifficulty << endl;
        }
        break;
      default:
        break;
    }
  }
}

Miner::Miner(const std::string uri, const unsigned int port, const std::string account, const std::string worker){
  this->pool_uri = uri;
  this->pool_port = port;
  this->account = account;
  this->worker = worker;
  net = new Net(uri, "", port);

  taskNonce = "";
  taskHeader = "";
  taskDifficulty = "";
  accepted = 0;
  rejected = 0;
}

void Miner::CalculateHash(){
  cout << "start calculate hash.." << endl;
  XCortex::XCortex xcortex; 
  srand((unsigned)time(0));
  XCortex::Hex hex;
  while(1){
    if(taskHeader != ""){
      cout << "header: " << taskHeader << endl;
      vector<unsigned char> header = hex.DecodeString(taskHeader.substr(2, taskHeader.size() - 2));      
      uint64_t nonce = rand();
      xcortex.set_header_nonce(header, nonce);
      uint8_t hash_result[32];
      xcortex.run(hash_result, sizeof(hash_result));

      cout << "hash result: "; 
      for(int i = 0; i < 32; i++){
        cout << (uint32_t)hash_result[i] << " ";
      }
      cout << endl;
      cout << "difficulty: ";
      vector<uint8_t> difficulty = hex.DecodeString(taskDifficulty.substr(2, taskDifficulty.size() - 2));
      for(int i = 0; i < 32; i++){
        cout << (uint32_t)difficulty[i] << " ";
      }
      cout << endl;
      if(memcmp(hash_result, difficulty.data(), sizeof(hash_result)) < 0){
        Submit(taskHeader, nonce);
      }
    }
    usleep(100);
  }       
}

void Miner::Run(){
  assert(net->Init());
  Login();

  std::thread t(&Miner::CalculateHash, this);

  char buffer[1024] = {'\0'};
  while(1){
    char tmpBuf[256];
    int ret = net->Recv(tmpBuf, sizeof(tmpBuf));
    if(ret <= 0){ // recv error or disconnect
      mut.lock();
      net->Close();
      net->Init();
      Login();
      mut.unlock();
    }else if(ret > 1 && tmpBuf[ret-1] == '\n'){
      tmpBuf[ret-1] = '\0';
      strcat(buffer, tmpBuf);
      cout << "recv: " << buffer << endl;
      Parse(buffer);
      buffer[0] = '\0';
    }else{
      strcat(buffer, tmpBuf); 
    }
    usleep(100);
  }
  t.join();
}

void Miner::Stop(){
  net->Close();
}

int main(int argc, char** argv){
  char *ip = "cuckoo.cortexmint.com";
  unsigned int port = 8008;
  string account = "0x23df8a88cab54a0e125bf8c6ed265372c32b2fe1";
  string worker = "";
  Miner miner(ip, port, account, worker);
  miner.Run();
  return 0;
}
