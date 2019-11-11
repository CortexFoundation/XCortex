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
        //TODO change value to BigEndian
        char *bytes = (char*)&value;
        return EncodeString(bytes);
      }
      std::string Uint32ArrayToHexString(std::vector<uint32_t>& values){
        //char *bytes = (char*)values;
        std::string str;
        str.resize(values.size()*4);
        for(int i = 0; i < values.size(); i++){
          char *p = (char*)&values[i];
          str[i*4+0] = p[0];
          str[i*4+1] = p[1];
          str[i*4+2] = p[2];
          str[i*4+3] = p[3];
        }
        return EncodeString(str);
      }
  };
};

#endif
