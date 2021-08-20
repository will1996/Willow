//
// Created by W on 9/25/20.
//

#ifndef GROVE_LUAENVIRONMENT_H
#define GROVE_LUAENVIRONMENT_H
#include "lua.hpp"
#include "willow/utils/Helpers.hpp"
#include"willow/data/Value.hpp"
#include"willow/scripting/LuaCommunication.hpp"
#include <string>
namespace wlo{
   class ScriptEnvironment {
   public:
       ScriptEnvironment();
       explicit ScriptEnvironment(lua_State* L);

        template <typename T>
        void setGlobal(std::string name, const T & v){
            m_stack.push(v);
            lua_setglobal(m_L,name.c_str());
        }

       template <typename T>
       T getGlobal(std::string name){
           lua_getglobal(m_L,name.c_str());
           T out = m_stack.pop(Data::type<T>()).template get<T>();
           lua_settop(m_L,0);//pop table
           return out;
       }

        template<typename T>
        T getTableEntry(std::string tableName,std::string entryName){
            lua_getglobal(m_L,tableName.c_str());
            lua_pushstring(m_L,entryName.c_str());
            lua_gettable(m_L,1);
            T out =  m_stack.pop(Data::type<T>()).template get<T>();
            lua_settop(m_L,0);
            return out;
        }

       void createTable(std::string name);//creates a table at global scope

        //create a Lua type from its c++ equivalent
       void declareType(data::Type equvalent);



       void runScript(std::string pathOrCode);
       bool isValidLuaFile(std::string path);


       std::string dumpstack();
       lua_State* getL();

       void LT_failOnNameConflict(std::string name);
       bool L_checklua( int res);

       lua_State * m_L;
       wlo::lua::Stack m_stack;
   };

}


#endif //GROVE_LUAENVIRONMENT_H
