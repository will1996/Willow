//
// Created by W on 9/25/20.
//

#ifndef GROVE_LUAENVIRONMENT_H
#define GROVE_LUAENVIRONMENT_H
#include "lua.hpp"
#include "willow/utils/Helpers.hpp"
#include "willow/root/Logger.hpp"

#include <string>
namespace wlo::lua{
   class Environment {
   public:
       Environment();
       Environment(lua_State* L);
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
       void LT_failOnNameConflict(std::string name);
       bool L_checklua( int res);

       lua_State * m_L;

   };

}


#endif //GROVE_LUAENVIRONMENT_H
