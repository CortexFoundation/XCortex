#include "net.hpp"
#include "json.hpp"
#include "../xcortex.hpp"
#include "../hex.hpp"
#include "miner.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <omp.h>

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
//  cout << "submit: nonce = " << nonce << ", header = " << header << ", difficulty = " << taskDifficulty << endl;
  //vector<uint32_t> sol(42, 0);
  //std::string solStr = "0x" + hex.Uint32ArrayToHexString(sol);
  //cout << "sol str = " << solStr << endl;
  std::string nonceStr = "0x" + XCortex::hex.Uint64ToHexString(nonce);

  ///string submit = "{\"id\":73,\"jsonrpc\":\"2.0\", \"method\":\"ctxc_submitWork\",\"params\":[\"" + nonceStr + "\",\"" + header +  "\",\"" + solStr + "\"],\"worker\":\"" + worker + "\"}\n";
  string submit = "{\"id\":73,\"jsonrpc\":\"2.0\",\"method\":\"ctxc_submitWork\",\"params\":[\""+ nonceStr + "\",\"" + header + "\",\"0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\"],\"worker\":\"\"}\n";
  mut.lock();
  net->Send(submit);
  mut.unlock();
 // cout << "submit: " << submit << endl; 
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
        //  cout << (isAccepted ? "share accepted" : "share reject") << endl;
        //  cout << "shared accepted=" << accepted << ", rejected=" << rejected << endl;
        }
        break;
      case 0:
      case 100:
        result = json.GetListString("result");
        if(result.size() >= 3){
          taskHeader = result[0];
          taskNonce = result[1];
          taskDifficulty = result[2];
          cout << "recv task: taskHeader = " << taskHeader << ", taskNonce = " << taskNonce << ", taskDifficulty = " << taskDifficulty << endl;
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
  net = new Net("", uri, port);

  taskNonce = "";
  taskHeader = "";
  taskDifficulty = "";
  accepted = 0;
  rejected = 0;
  hashs = 0;
}

void Miner::CalculateHash(){
  cout << "start calculate hash.." << endl;
  XCortex::XCortex xcortex; 
  srand((unsigned)time(0));
  Random xcortex_random;
  while(1){
    if(taskHeader != ""){
      //cout << "header: " << taskHeader << endl;
      vector<unsigned char> header = XCortex::hex.DecodeString(taskHeader.substr(2, taskHeader.size() - 2));      
      uint64_t nonce = rand();
      xcortex.set_header_nonce(header, nonce);
      uint8_t hash_result[32];
      xcortex.run(hash_result, sizeof(hash_result), xcortex_random);
      ++hashs;

//      cout << "hash result: "; 
//      for(int i = 0; i < 32; i++){
//        cout << (uint32_t)hash_result[i] << " ";
//      }
//      cout << endl;
//      cout << "difficulty: ";
      vector<uint8_t> difficulty = XCortex::hex.DecodeString(taskDifficulty.substr(2, taskDifficulty.size() - 2));
//      for(int i = 0; i < 32; i++){
//        cout << (uint32_t)difficulty[i] << " ";
//      }
//      cout << endl;
      if(memcmp(hash_result, difficulty.data(), sizeof(hash_result)) < 0){
        Submit(taskHeader, nonce);
      }
    }
  }       
}
void Miner::PrintHashRate(){
  double start = omp_get_wtime();
  while(1){
    sleep(1);
    double time = omp_get_wtime() - start;
    cout << "hash rate = " << hashs / time << ", share accepted=" << accepted << ", share rejected=" << rejected << endl; 
  }
}

void Miner::Run(){
  assert(net->Init());
  Login();

  std::thread thread_calculate_hash(&Miner::CalculateHash, this);
  std::thread thread_print_hash_rate(&Miner::PrintHashRate, this);

  //char buffer[1024] = {'\0'};
  string buffer;
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
      //strcat(buffer, tmpBuf);
      buffer += string(tmpBuf);
      //cout << "recv: " << buffer << endl;
      Parse(buffer.c_str());
      //buffer[0] = '\0';
      buffer.clear();
    }else{
      //strcat(buffer, tmpBuf); 
      buffer += string(tmpBuf);
    }
    usleep(100);
  }
  thread_calculate_hash.join();
  thread_print_hash_rate.join();
}

void Miner::Stop(){
  net->Close();
}

int main(int argc, char** argv){
  char *ip = "127.0.0.1";
  unsigned int port = 8008;
  string account = "0x23df8a88cab54a0e125bf8c6ed265372c32b2fe1";
  string worker = "";
  Miner miner(ip, port, account, worker);
  miner.Run();
  return 0;
}
