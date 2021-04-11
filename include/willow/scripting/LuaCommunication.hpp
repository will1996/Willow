//
// Created by W on 12/19/20.
//

#ifndef WILLOW_LUAFUNCTION_HPP
#define WILLOW_LUAFUNCTION_HPP
#include "lua.hpp"
#include "willow/data/Value.hpp"
namespace wlo::lua{
    class Stack{
    public:
        explicit Stack(lua_State * L);

        void push(data::Value toPush);//push a value onto the stack
        static void push(lua_State * L, data::Value toPush);
        data::Value pop(data::Type expected);//pop a value from the stack, providing an expected type
        static data::Value pop(lua_State* L, data::Type expected);

        void print();
        static void print(lua_State *L );

        bool isEmpty();
        static bool isEmpty(lua_State *L );

    private:
        lua_State * m_state;
    };
}
#endif //WILLOW_LUAFUNCTION_HPP
