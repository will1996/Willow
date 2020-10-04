#pragma once
#ifndef wilo_scriptable_h
#define wilo_scriptable_h
#include "lua.hpp"
#include "willow/root/wilo_dev_core.hpp"
#include<iostream>
#include "willow/messaging/wilo_subject.hpp"
#include "willow/messaging/wilo_observer.hpp"
#include "LuaBasics.h"
//TODO add Cmake defined install settings header, which specifies scriptable base script
namespace wlo{

    
    //TODO implement tracking of scriptables in the global environment (maybe)

        template <class T>
        class Scriptable {
            //static std::vector<std::pair<std::string,lua_State*>> Instances;
        public:

            Scriptable(std::string name, T* instance_ptr,wlo::SharedPointer<wlo::lua::Environment> env): m_env(env),m_name(name),m_instancePtr(instance_ptr){
                runBaseScript();
                LT_instantiate();
            }


            virtual ~Scriptable() {}
        protected:
            //Grant the oppourtunity to refine the Lua environment that is coarsely
            //defined by the LT methods. Since this Lua script has access to 
            //all of the globals henceforth defined in the lua state, we can do 
            //almost anything we want here, it is intended to restrict access 
            //and built structure into the otherwise almost completely plastic 
            //Lua environemtn
            virtual void initialize() = 0;
            wlo::SharedPointer<wlo::lua::Environment> m_env;
            const std::string m_name;

            template< int(T::* Method)(lua_State*)>
            //add the member function in the template to this Luatable with the name "name"
            void LT_appendMember(std::string name) {
                lua_getglobal(m_env->getL(), m_name.c_str());//retrieve the class table from the global namespace

                lua_pushstring(m_env->getL(), name.c_str());//push a key value pair of the string name
                lua_pushcfunction(m_env->getL(), call < Method >);//and the function call<Method>

                lua_settable(m_env->getL(), -3);//push the pair into the table

                lua_setglobal(m_env->getL(), m_name.c_str());//update the global variable with the new entries
            }
            /*create or add to a lua table, with string pairs. Useful for pretty much just sending paths... */

#ifndef ndebug
            void L_reinitialize(){
                runBaseScript();
            }
#endif
        private:
            T* m_instancePtr; 
        //invoke the template method on the 'this' pointer, calling a non-static member function
            template<int(T::*Method)(lua_State* L) >
        static int call(lua_State* L){
            lua_pushstring(L,"instancePtr");
            lua_gettable(L,1);
            T* this_ptr = static_cast<T*> (lua_touserdata(L,-1));
            lua_pop(L, 1);//pop the user_data "this" pointer from the top of the stack
            lua_remove(L, 1);//get rid of the call table, while preserving arguments
            return (this_ptr->*Method)(L);
        }

        void runBaseScript(){
            const std::string scriptpath(WILO_ENGINE_SCRIPTS_PATH);
            int res = (luaL_dofile(m_env->getL(), (scriptpath+"scriptable_base.lua").c_str()));
            if(res != LUA_OK) {
                #ifndef NDEBUG
                std::string msg = lua_tostring(m_env->getL(),-1);
                WILO_CORE_ERROR("invalid Lua_base script, failed with:  "+ msg);
                #else    
                raise runtime_error("invalid Lua_base script please validate your install");
                #endif
            }
            
        }

      void LT_failOnNameConflict(){
            lua_getglobal(m_env->getL(), m_name.c_str());
            
            if (! lua_isnil(m_env->getL(), -1) ) {
                throw std::runtime_error("invalid Lua configuration, name " + m_name + " is already in this lua Namespace");
            }
        }

        //build a global table in Lua with the name m_name, and a reference to this object
        //failing if a global table of the same name already exists
        void LT_instantiate(){
            std::cout << "instantiating: " << m_name << "in the lua global environment!"<<std::endl;
            //LT_failOnNameConflict();

           lua_newtable(m_env->getL());//push a fresh table onto the stack

           lua_pushstring(m_env->getL(), "instancePtr");//push a key value pair of the string "this"
           lua_pushlightuserdata(m_env->getL(), m_instancePtr);// and the value a pointer to 'this'

           lua_settable(m_env->getL(), 1);//add the pair to the table
           std::cout << std::flush;
           lua_setglobal(m_env->getL(), m_name.c_str());//name the table, and pop it from the stack
        }

        void LT_destruct(){
            lua_pushnil(m_env->getL());
            lua_setglobal(m_env->getL(), m_name.c_str());
        } 

    };


}
#endif
