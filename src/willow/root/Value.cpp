//
// Created by Will Chambers on 4/3/21.
//
#include"willow/data/Value.hpp"

namespace wlo::data{


//copy constructor first copies out the core of the Value, (the actual data), then it builds a set of members pointing to the core
Value::Value(const Value & other):
m_self(other.m_self->copy()),m_members(buildMembers(m_self.get(),other.getType())){ }

Value::Value(const Value &&other):m_self(other.m_self->copy()),m_members(buildMembers(m_self.get(),other.getType()))
{ }

Value::Value(std::vector<Value> init){
    std::vector<Type::Member> typeInit; typeInit.reserve(init.size());
    for (size_t i =0; i < init.size(); i++)
        typeInit.push_back({.name = std::to_string(i),.type = init[i].getType()});
    Type t("",typeInit);
    m_self = CreateUniquePointer<Model<Type>>(t);
    m_members = buildMembers(m_self.get(),t);
    for (size_t i =0; i < init.size(); i++)
        m_members[std::to_string(i)] = init[i];
}


Value::Value(std::vector<std::pair<std::string, Value>> init) {
    std::vector<Type::Member> typeInit; typeInit.reserve(init.size());
    for (const auto [name ,value ] : init)
        typeInit.push_back({name,value.getType()});
    Type t("",typeInit);
    m_self = CreateUniquePointer<Model<Type>>(t);
    m_members = buildMembers(m_self.get(),t);
    for (const auto [name ,value ] : init)
        m_members[name] = value;
}



Value::Value(const Type &t):m_self(CreateUniquePointer<Model<Type>>(t)),m_members(buildMembers(m_self.get(),t)) {

}


Value::Value(const Type&  type, void * src):m_self(CreateUniquePointer<Model<void*>>(type,src)){
    m_members = buildMembers(m_self.get(),type);
}

Value::Value():m_self(CreateUniquePointer<Model<void*>>(typeOf<void>(),nullptr)){ }

Value & Value::operator[](const std::string & memberName) {
    return m_members.at(memberName);
}
Value &Value::operator[](size_t i) {
    return m_members.at(getType().getMembers()[i].name);
}

Value& Value::operator=(const Value&& other)
{
    m_self->Assign(other.m_self.get());
    return *this;
}

 Value &Value::operator=(const Value &other) {
     m_self->Assign(other.m_self.get());
     return *this;
 }

std::map<std::string, Value> Value::buildMembers(Value::Abstract * storage, const Type & t) {
   std::map<std::string,Value> members;
   for(auto & member: t.getMembers()){
       byte * attachmentPoint = &static_cast<byte*>(storage->getData())[member.offset];
       members[member.name] = Value(member.type,attachmentPoint);
        }
    return members;
    }




}
