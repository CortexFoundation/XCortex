#ifndef MINER_JSON_H
#define MINER_JSON_H

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include <iostream>
#include <vector>
using namespace rapidjson;

class Json{
  Document document;
  public:
    Json(){};
    Json(const char*strJson){
      document.Parse(strJson);
    }
    void Parse(const char*strJson){
      document.Parse(strJson);
    }
    bool IsJson(){
      return document.IsObject();
    }
    std::string GetString(const char* key){
      assert(document.HasMember(key));
      return document[key].GetString();
    }
    int GetInt(const char *key){
      assert(document.HasMember(key));
      return document[key].GetInt();
    }
    double GetDouble(const char *key){
      assert(document.HasMember(key));
      return document[key].GetDouble();
    }
    bool GetBool(const char *key){
      assert(document.HasMember(key));
      return document[key].GetBool();
    }
    bool IsNull(const char* key){
      return document.HasMember(key) ? document[key].IsNull() : true;
    }
    std::vector<std::string> GetListString(const char *key){
      assert(document.HasMember(key));
      const Value& array = document[key];
      assert(array.IsArray());
      
      std::vector<std::string> out(array.Size());
      for(SizeType i = 0; i < array.Size(); i++){
        out[i] = array[i].GetString(); 
      }
      return out;
    }
};

#endif
