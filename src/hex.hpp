#ifndef XCORTEX_HEX_H
#define XCORTEX_HEX_H

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdint.h>

namespace XCortex{
  class Hex{
    public:
      std::string EncodeString(const std::string& str){
        std::ostringstream strHex;
        strHex << std::hex << std::setfill('0');
        for(size_t i = 0; i < str.size(); i++){
          strHex << std::setw(2) << static_cast<unsigned int>(str[i]);
        }
        return strHex.str();
      }

      std::vector<unsigned char> DecodeString(const std::string &hex)
      {
        std::vector<unsigned char> dest;
        auto len = hex.size();
        dest.reserve(len / 2);
        for (decltype(len) i = 0; i < len; i += 2)
        {
          unsigned int element;
          std::istringstream strHex(hex.substr(i, 2));
          strHex >> std::hex >> element;
          dest.push_back(static_cast<unsigned char>(element));
        }
        return dest;
      }

      std::string Uint64ToHexString(uint64_t value){
        //TODO little endian and big endian
        std::stringstream ss;
        ss << std::setw(16) << std::setfill('0') << std::hex << value;
        return ss.str();
      }
      std::string Uint32ToHexString(uint32_t value){
        //TODO little endian and big endian
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << std::hex << value;
        return ss.str();
      }
      std::string Uint32ArrayToHexString(std::vector<uint32_t>& values){
        //char *bytes = (char*)values;
        std::string str;
        str.resize(values.size()*4);
        for(int i = 0; i < values.size(); i++){
          str += Uint32ToHexString(values[i]);
        }
        return str;
      }
      uint64_t HexStringToUint64(std::string& str){
        char* endptr;
        uint64_t ret = strtoull(str.c_str(), &endptr, 16);
        if(endptr == str.c_str()){
          std::cout << "hex str to uint64 : invalid input " << 0 << std::endl;
        }else if (endptr != NULL && *endptr != '\0'){
          std::cout << "hex str to uint64 : invalid input " << 1 << std::endl; 
        }else{
          //std::cout << "valid input" << std::endl;
        }
        return ret;
      }
      uint32_t HexStringToUint32(std::string& str){
        char* endptr;
        uint32_t ret = strtoul(str.c_str(), &endptr, 16);
        if(endptr == str.c_str()){
          std::cout << "hex str to uint32: invalid input " << 0 << std::endl;
        }else if (endptr != NULL && *endptr != '\0'){
          std::cout << "hex str to uint32: invalid input " << 1 << std::endl; 
        }else{
          //std::cout << "valid input" << std::endl;
        }
        return ret;
      }
  };
  static Hex hex;
};

#endif
