#ifndef MINER_NET_H
#define MINER_NET_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

class Net{
  std::string ip;
  std::string domain;
  unsigned int port;
  int socket_fd;
  
  public:
    Net(std::string domain, std::string ip, unsigned int port){
      this->domain = domain;
      this->ip = ip;
      this->port = port;
    }

    std::string GetErrorString(){
      return strerror(errno);
    }
    bool Init(){
      std::cout << "init ...." << std::endl;
      socket_fd = socket(AF_INET, SOCK_STREAM, 0);
      if(socket_fd < 0){
        return false;
      }

      struct sockaddr_in servaddr;
      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_port = htons(port);
      if(ip.empty()){
        struct hostent *hptr = gethostbyname(domain.c_str());
        if(hptr == NULL){
          return false;
        }
        servaddr.sin_addr.s_addr = *((unsigned long long *)hptr->h_addr_list[0]);
      }else{
        if(inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr) <= 0){
          return false;
        }
      }

      if(connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        return false;
      }
      return true;
    }

    int Send(std::string buffer){
      int ret = send(socket_fd, buffer.c_str(), buffer.size(), 0);
      return ret;
    }
    int Recv(char* buffer, unsigned int size){
      int ret = recv(socket_fd, buffer, size, 0);
      return ret;
    }
    void Close(){
      close(socket_fd);
    }
};

#endif
