#include "lua.hpp"
#include "willow/root/Root.hpp"
#include<iostream>
#include "LuaEnvironment.hpp"
#include"willow/root/FileSystem.hpp"
#include"willow/data/Type.hpp"
#include"willow/data/Value.hpp"
#include"LuaCommunication.hpp"
#pragma once
namespace wlo{
        template <class T>
        class LuaBinding {
            //static std::vector<std::pair<std::string,lua_State*>> Instances;
        public:

            LuaBinding(std::string name, T* instance_ptr, wlo::ScriptEnvironment& env): m_env(env), m_name(name), m_instancePtr(instance_ptr){
                LT_instantiate();
            }
            template< void(T::* Method)()>
            //add the member function in the template to this Luatable with the name "name"
            void Register(std::string name) {
                lua_getglobal(m_env.getL(), m_name.c_str());//retrieve the class table from the global namespace

                lua_pushstring(m_env.getL(), name.c_str());//push a key value pair of the string name
                lua_pushcclosure(m_env.getL(), call <Method>,0);//and the function call<Method>

                lua_settable(m_env.getL(), -3);//push the pair into the table

                lua_setglobal(m_env.getL(), m_name.c_str());//update the global variable with the new entries
            }

            template< typename A,void(T::* Method)(A)>
            //add the member function in the template to this Luatable with the name "name"
            void Register(std::string name) {
                lua_getglobal(m_env.getL(), m_name.c_str());//retrieve the class table from the global namespace

                lua_pushstring(m_env.getL(), name.c_str());//push a key value pair of the string name
                lua_pushcclosure(m_env.getL(), call <Method,A>,0);//and the function call<Method>

                lua_settable(m_env.getL(), -3);//push the pair into the table

                lua_setglobal(m_env.getL(), m_name.c_str());//update the global variable with the new entries
            }
            template< typename A,typename A2,void(T::* Method)(A,A2)>
            //add the member function in the template to this Luatable with the name "name"
            void Register(std::string name) {
                lua_getglobal(m_env.getL(), m_name.c_str());//retrieve the class table from the global namespace

                lua_pushstring(m_env.getL(), name.c_str());//push a key value pair of the string name
                lua_pushcclosure(m_env.getL(), call<Method,A,A2>,0);//and the function call<Method>

                lua_settable(m_env.getL(), -3);//push the pair into the table

                lua_setglobal(m_env.getL(), m_name.c_str());//update the global variable with the new entries
            }

            template< typename A,typename A2,typename A3,void(T::* Method)(A,A2,A3)>
            //add the member function in the template to this Luatable with the name "name"
            void Register(std::string name) {
                lua_getglobal(m_env.getL(), m_name.c_str());//retrieve the class table from the global namespace

                lua_pushstring(m_env.getL(), name.c_str());//push a key value pair of the string name
                lua_pushcclosure(m_env.getL(), call<Method,A,A2,A3>,0);//and the function call<Method>

                lua_settable(m_env.getL(), -3);//push the pair into the table

                lua_setglobal(m_env.getL(), m_name.c_str());//update the global variable with the new entries
            }

            ScriptEnvironment & getEnv() {
                    return m_env;
            }



            virtual ~LuaBinding() {
                LT_destruct();
            }
        protected:
            //Grant the oppourtunity to refine the Lua environment that is coarsely
            //defined by the LT methods. Since this Lua script has access to
            //all of the globals henceforth defined in the lua state, we can do
            //almost anything we want here, it is intended to restrict access
            //and built structure into the otherwise almost completely plastic
            //Lua environemtn
            wlo::ScriptEnvironment& m_env;
            const std::string m_name;


#ifndef ndebug
            void L_reinitialize(){
            }
#endif
        private:
            T* m_instancePtr;
        //invoke the template method on the 'this' pointer, calling a non-static member function


            template<auto Method>
            static int call(lua_State* L){
                lua_pushstring(L,"instancePtr");
                lua_gettable(L,1);
                T* this_ptr = static_cast<T*> (lua_touserdata(L,-1));
                lua_pop(L, 1);//pop the user_data "this" pointer from the top of the stack
                lua_remove(L, 1);//get rid of the call table, while preserving arguments
                (this_ptr->*Method)();
                return 0;
            }

            template<auto Method, typename A >
        static int call(lua_State* L){
            lua_pushstring(L,"instancePtr");
            lua_gettable(L,1);
            T* this_ptr = static_cast<T*> (lua_touserdata(L,-1));
            lua_pop(L, 1);//pop the user_data "this" pointer from the top of the stack
            lua_remove(L, 1);//get rid of the call table, while preserving arguments
            auto arg = lua::Stack::pop(L,Data::type<A>()).template get<A>();
            (this_ptr->*Method)(arg);
            return 0;
        }
            template<auto Method, typename A ,typename A2>
            static int call(lua_State* L){
                lua_pushstring(L,"instancePtr");
                lua_gettable(L,1);
                T* this_ptr = static_cast<T*> (lua_touserdata(L,-1));
                lua_pop(L, 1);//pop the user_data "this" pointer from the top of the stack
                lua_remove(L, 1);//get rid of the call table, while preserving arguments
                lua::Stack::print(L);
                auto arg2 = lua::Stack::pop(L,Data::type<A2>()).template get<A2>();
                auto arg1 = lua::Stack::pop(L,Data::type<A>()).template get<A>();
                assert(lua::Stack::isEmpty(L));
                (this_ptr->*Method)(arg1,arg2);
                return 0;
            }

            template<auto Method, typename A ,typename A2,typename A3>
            static int call(lua_State* L){
                lua_pushstring(L,"instancePtr");
                lua_gettable(L,1);
                T* this_ptr = static_cast<T*> (lua_touserdata(L,-1));
                lua_pop(L, 1);//pop the user_data "this" pointer from the top of the stack
                lua_remove(L, 1);//get rid of the call table, while preserving arguments
                auto arg3 = lua::Stack::pop(L,Data::type<A3>()).template get<A3>();
                auto arg2 = lua::Stack::pop(L,Data::type<A2>()).template get<A2>();
                auto arg1 = lua::Stack::pop(L,Data::type<A>()).template get<A>();
                (this_ptr->*Method)(arg1,arg2,arg3);
                return 0;
            }

        void runBaseScript(){
            int res = luaL_dofile(m_env.getL(), (wlo::FileSystem::Script("scriptable_base.lua").string().c_str()));
            if(res != LUA_OK) {
                #ifndef NDEBUG
                std::string msg = lua_tostring(m_env.getL(),-1);
                #else
//                raise runtime_error("invalid Lua_base script please validate your install");
                #endif
            }

        }

      void LT_failOnNameConflict(){
            lua_getglobal(m_env.getL(), m_name.c_str());

            if (! lua_isnil(m_env.getL(), -1) ) {
                throw std::runtime_error("invalid Lua configuration, name " + m_name + " is already in this lua Namespace");
            }
        }

        //build a global table in Lua with the name m_name, and a reference to this object
        //failing if a global table of the same name already exists
        void LT_instantiate(){
           //LT_failOnNameConflict();
 //          WILO_CORE_INFO("LuaBinding for {0} ...",m_name);
           lua_newtable(m_env.getL());//push a fresh table onto the stack
           lua_pushstring(m_env.getL(), "instancePtr");//push a key value pair of the string "this"
           lua_pushlightuserdata(m_env.getL(), m_instancePtr);// and the value a pointer to 'this'
           lua_settable(m_env.getL(), 1);//add the pair to the table
           lua_setglobal(m_env.getL(), m_name.c_str());//name the table, and pop it from the stack
//           WILO_CORE_INFO("LuaBinding established for {0}",m_name);
           lua_settop(m_env.getL(),0);
        }

        void LT_destruct(){
            lua_pushnil(m_env.getL());
            lua_setglobal(m_env.getL(), m_name.c_str());
        }
    };


}
