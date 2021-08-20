//
// Created by W on 10/2/20.
//
#include "willow/scripting/LuaEnvironment.hpp"
#include "willow/root/Root.hpp"
namespace wlo{
   ScriptEnvironment::ScriptEnvironment(): m_L(luaL_newstate()),m_stack(m_L) {
    luaL_openlibs(m_L);
   }

    ScriptEnvironment::ScriptEnvironment(lua_State* L): m_L(L),m_stack(m_L){
    luaL_openlibs(m_L);
    }
    lua_State* ScriptEnvironment::getL(){
       return m_L;
   }

    void ScriptEnvironment::LT_failOnNameConflict(std::string name){
        lua_getglobal(m_L, name.c_str());
        if (! lua_isnil(m_L, -1) ) {
            throw std::runtime_error("invalid Lua configuration, name " + name + " is already in this lua Namespace");
        }
    }

    void ScriptEnvironment::runScript(std::string pathOrCode) {
        if (isValidLuaFile(pathOrCode)){
            L_checklua(luaL_dofile(m_L, pathOrCode.c_str()));
        }
        else {
            int res = luaL_dostring(m_L, pathOrCode.c_str());
            L_checklua(res);
        }
    }
    bool ScriptEnvironment::isValidLuaFile(std::string path){
        return wlo::helpers::endswith(path, ".lua") && wlo::helpers::fexists(path);
    }

    std::string ScriptEnvironment::dumpstack(){
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

    bool ScriptEnvironment::L_checklua(int res) {
        if (res != LUA_OK) {
            dumpstack();
            std::string errmsg = lua_tostring(m_L, -1);
            lua_pop(m_L,-1);
            return false;
        }
        return true;
    }

}


