//
// Created by W on 9/25/20.
//

#ifndef WILLOW_HELPERS_H
#define WILLOW_HELPERS_H
#include<fstream>
#include<string>
#include<sstream>

namespace wlo::helpers{
  inline  bool fexists(std::string path) {
        std::ifstream stream(path.c_str());
        return stream.good();
   }


   inline bool endswith(std::string str,std::string suffix){
           if (str.length() >= suffix.length()) {
               return (0 == str.compare (str.length() - suffix.length(), suffix.length(), suffix));
           } else {
               return false;
           }
       }

  inline size_t toNumber(const std::string & str){
        std::stringstream ss(str);
        size_t value;
        ss>>value;
        return value;
  }
}


#endif //WILLOW_HELPERS_H
