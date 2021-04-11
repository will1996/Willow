//
// Created by Will Chambers on 4/3/21.
//

#include "willow/scripting/LuaCommunication.hpp"


namespace wlo::lua{

    bool isInValidSet(const data::Type & expected){
        static const vector<data::Type> goodTypes = {
                data::Type::of<int>(),
                data::Type::of<float>(),
                data::Type::of<double>(),
                data::Type::of<std::string>()
        };
        bool OK = false;
        for(const auto & type : goodTypes)
            if(expected==type)
                OK = true;

        return OK;
    }
    //types to lua must be composed of types which on some level are represented by floats, ints, or strings.
    void validate(const data::Type &attemptedType){
        if(attemptedType.isPrimitive()||attemptedType==data::Type::of<std::string>()){
            if(! isInValidSet(attemptedType))
                throw std::invalid_argument("Attempted to pass a type containing a primitive of  type: " + attemptedType.name() + " to or from Lua, which is unsupported");
        }
        for(const auto & member : attemptedType.getMembers()){
            validate(member.type);
        }
    }

    Stack::Stack(lua_State *L):m_state(L){

    }

    template<typename T>
    void push(lua_State* L, data::Value toPush){
        throw std::logic_error("non-specialized Push is undefined! must specialze with a supported Lua stack type");
    }

    template<>
    void push<float>(lua_State*L,data::Value valToPush){
        assert(data::Type::of<float>()==valToPush.getType());
        float toPush = valToPush.get<float>();
        lua_pushnumber(L,toPush);
    }
    template<>
    void push<int>(lua_State*L,data::Value valToPush){
        assert(data::Type::of<int>()==valToPush.getType());
        int toPush = valToPush.get<int>();
        lua_pushinteger(L,toPush);
    }
    template<>
    void push<double>(lua_State*L,data::Value valToPush){
        assert(data::Type::of<double>()==valToPush.getType());
        double toPush = valToPush.get<double>();
        lua_pushnumber(L,toPush);
    }

    template<>
    void push<std::string>(lua_State*L,data::Value valToPush){
        assert(data::Type::of<std::string>()==valToPush.getType());
        std::string toPush = valToPush.get<std::string>();
        lua_pushstring(L,toPush.c_str());
    }

    void Stack::push(data::Value toPush) {
        push(m_state,toPush);
    }

    void Stack::push(lua_State *L, data::Value toPush) {
        validate(toPush.getType());
        if(toPush.getType()==data::Type::of<int>())
            return lua::push<int>(L,toPush);
        if(toPush.getType()==data::Type::of<float>())
           return  lua::push<float>(L,toPush);
        if(toPush.getType()==data::Type::of<double>())
            return lua::push<double>(L,toPush);
        if(toPush.getType()==data::Type::of<std::string>())
            return lua::push<std::string>(L,toPush);
        //composite types first create a table, then recusively push members, and set them into the table.
        lua_newtable(L);
        for(auto member :toPush.getType().getMembers()){
            lua_pushstring(L,member.name.c_str());
            push(L,toPush[member.name]);
            lua_settable(L,1);
        }


    }

    data::Value Stack::pop(data::Type expected) {
        return pop(m_state,expected);
    }



    template<typename T>
    T pop(lua_State *L){
       throw std::logic_error("non-specialized Pop is undefined! must specialze with a supported Lua stack type");
    }

    template<>
    float pop<float>(lua_State*L){
       if(! lua_isnumber(L,-1))
           throw std::invalid_argument("Attempted to pop a float, but top of Lua stack is other");
      float ret = lua_tonumber(L,-1);
      lua_remove(L,-1);
      return ret;
    }

    template<>
    double pop<double>(lua_State*L){
        if(! lua_isnumber(L,-1))
            throw std::invalid_argument("Attempted to pop a double, but top of Lua stack is other");
        double ret = lua_tonumber(L,-1);
        lua_remove(L,-1);
        return ret;
    }

    template<>
    int pop<int>(lua_State*L){
        if(! lua_isinteger(L,-1))
            throw std::invalid_argument("Attempted to pop an int, but top of Lua stack is other");
        int ret = lua_tointeger(L,-1);
        lua_remove(L,-1);
        return ret;
    }

    template<>
    std::string pop<std::string>(lua_State*L){
        if(! lua_isstring(L,-1))
            throw std::invalid_argument("Attempted to pop a string, but top of Lua stack is other");
        std::string ret = lua_tostring(L,-1);
        lua_remove(L,-1);
        return ret;
    }


    data::Value Stack::pop(lua_State *L, data::Type expected) {
        validate(expected);
        if(expected==data::Type::of<std::string>())
            return data::Value(lua::pop<std::string>(L));
        if(expected==data::Type::of<float>())
            return data::Value(lua::pop<float>(L));
        if(expected==data::Type::of<int>())
            return data::Value(lua::pop<int>(L));
        data::Value v(expected);
        for(auto [name,type,offset]:expected.getMembers()){
            lua_pushstring(L,name.c_str());
            lua_gettable(L,1);
            v[name] = pop(L,type);
        }
        lua_remove(L,-1);//pop the table
        assert(isEmpty(L));
        return v;
    }

    void Stack::print() {
        print(m_state);
    }

    void Stack::print(lua_State *L) {
            int i;
            int top = lua_gettop(L);
            for (i = 1; i <= top; i++) {  /* repeat for each level */
                int t = lua_type(L, i);
                switch (t) {

                    case LUA_TSTRING:  /* strings */
                        printf("`%s'", lua_tostring(L, i));
                        break;

                    case LUA_TBOOLEAN:  /* booleans */
                        printf(lua_toboolean(L, i) ? "true" : "false");
                        break;

                    case LUA_TNUMBER:  /* numbers */
                        printf("%g", lua_tonumber(L, i));
                        break;

                    default:  /* other values */
                        printf("%s", lua_typename(L, t));
                        break;

                }
                printf("  ");  /* put a separator */
            }
            printf("\n");  /* end the listing */
        }

    bool Stack::isEmpty() {
        return isEmpty(m_state);
    }

    bool Stack::isEmpty(lua_State *L) {
        return lua_gettop(L)==0;
    }
}