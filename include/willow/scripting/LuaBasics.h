//
// Created by W on 9/25/20.
//

#ifndef GROVE_LUAENVIRONMENT_H
#define GROVE_LUAENVIRONMENT_H
#include "lua.hpp"
#include "willow/dev-utils/Helpers.h"
#include "willow/root/logr.hpp"
#include <string>
namespace wlo::lua{
   class Environment {
   public:
       Environment();
       void setglobal(std::string name, std::string data);
       void setglobal(std::string name, int data);
       void setglobal(std::string name, float data);
       void setglobal(std::string name, double data);
       auto getglobal(std::string name);
       void runScript(std::string pathOrCode);
       bool isValidLuaFile(std::string path);
       std::string dumpstack();
       lua_State* getL();

   private:
       bool L_checklua( int res);

       lua_State * m_L;

   };

}


#endif //GROVE_LUAENVIRONMENT_H
