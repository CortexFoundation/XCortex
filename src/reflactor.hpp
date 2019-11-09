#ifndef XCORTEX_REFLECTOR_H
#define XCORTEX_REFLECTOR_H

#include <map>
#include <vector>
#include <memory>

namespace XCortex{
#define FUNC std::function<void*(void)>
  class Reflector{
    private:
      std::map<std::string, FUNC> objectMap;
      std::vector<FUNC> objectList;
      static std::shared_ptr<Reflector> ptr;
    public:
      void* CreateObject(const std::string& class_name){
        //return objectMap[class_name];
        for(auto& x : objectMap){
          if(x.first == class_name) return x.second();
        }
        return nullptr;
      }
      void Register(const std::string& class_name, FUNC && generator){
        objectMap[class_name] = generator; 
        objectList.push_back(generator);
      }
      std::vector<void*> ObjectList(){
        std::vector<void*> list(objectList.size());
        int i = 0;
        for(auto& x : objectList){
          list[i++] = x();
        }
        return list;
      }
      static std::shared_ptr<Reflector> Instance(){
        if(ptr == nullptr){
          ptr.reset(new Reflector());
        }
        return ptr;
      }
  };


  std::shared_ptr<Reflector> Reflector::ptr = nullptr;

  class Register{
    public:
      Register(const std::string& class_name, FUNC&& generator){
        Reflector::Instance()->Register(class_name, std::forward<FUNC>(generator));
      }
  };

#define REGISTEROP(CLASS_NAME) \
  Register g_register_##CLASS_NAME(#CLASS_NAME, []()\
      { return new CLASS_NAME(); });

#define REGISTEROP_BY_NAME(CLASS_NAME, OP_NAME) \
  Register g_register_##CLASS_NAME##_##OP_NAME(#CLASS_NAME, [](){ return new CLASS_NAME(#OP_NAME); });
};

#endif
