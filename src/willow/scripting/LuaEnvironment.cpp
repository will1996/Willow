//
// Created by W on 10/2/20.
//
#include "willow/scripting/LuaEnvironment.h"
#include "willow/root/wilo_dev_core.hpp"
namespace wlo::lua{
   Environment::Environment():m_L(luaL_newstate()) {
    luaL_openlibs(m_L);
   }

    Environment::Environment(lua_State* L):m_L(L){
    luaL_openlibs(m_L);
    }
    lua_State* Environment::getL(){
       return m_L;
   }

    void Environment::LT_failOnNameConflict(std::string name){
        lua_getglobal(m_L, name.c_str());

        if (! lua_isnil(m_L, -1) ) {
            throw std::runtime_error("invalid Lua configuration, name " + name + " is already in this lua Namespace");
        }
    }

    void Environment::setglobal(std::string name, std::string data) {
        lua_pushstring(m_L, data.c_str());
        lua_setglobal(m_L, name.c_str());
    }
    void Environment::setglobal(std::string name, int data) {
        lua_pushnumber(m_L, data);
        lua_setglobal(m_L, name.c_str());
    }
    void Environment::setglobal(std::string name, float data) {
        lua_pushnumber(m_L, data);
        lua_setglobal(m_L, name.c_str());
    }
    void Environment::setglobal(std::string name, double data) {
        lua_pushnumber(m_L, data);
        lua_setglobal(m_L, name.c_str());
    }
    void Environment::runScript(std::string pathOrCode) {
        if (isValidLuaFile(pathOrCode)){
            L_checklua(luaL_dofile(m_L, pathOrCode.c_str()));
        }
        else
            L_checklua(luaL_dostring(m_L, pathOrCode.c_str()));
    }
    bool Environment::isValidLuaFile(std::string path){
        return wlo::helpers::endswith(path, ".lua") && wlo::helpers::fexists(path);
    }

    std::string Environment::dumpstack(){
        std::stringstream ss;
        int i;
        int top = lua_gettop(m_L);
        for (i = 1; i <= top; i++) {  /* repeat for each level */
            int t = lua_type(m_L, i);
            switch (t) {

                case LUA_TSTRING:  /* strings */
                    ss<<lua_tostring(m_L, i);
                    break;

                case LUA_TBOOLEAN:  /* booleans */
                    ss<<(lua_toboolean(m_L, i) ? "true" : "false");
                    break;

                case LUA_TNUMBER:  /* numbers */
                    ss<< lua_tonumber(m_L, i);
                    break;

                default:  /* other values */
                    ss<< lua_typename(m_L, t);
                    break;

            }
            ss<<"  ";  /* put a separator */
        }
        ss<<std::endl;  /* end the listing */
        return ss.str();

    }

    bool Environment::L_checklua( int res) {
        if (res != LUA_OK) {
            std::string errmsg = lua_tostring(m_L, -1);
            lua_pop(m_L,-1);
            WILO_CORE_ERROR("[LUA] "+errmsg);
            return false;
        }
        return true;
    }
}


