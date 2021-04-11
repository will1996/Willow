//
// Created by W on 12/6/20.
//
#include"willow/data/Type.hpp"

namespace wlo::data {

    Type::Type() :
            m_size(0) ,m_name("void"),m_isPrimitve(true){}

    Type::Type(std::string name, size_t size):
    m_name(std::move(name)),
    m_size(size),
    m_members({}),
    m_isPrimitve(true),
    m_isContainer(false),
    m_isComposite(false)
    {

    }


    Type::Type(std::string name, std::vector<Member> members):
    m_name(std::move(name)),
    m_members(std::move(members)),
    m_size(0),
    m_isPrimitve(false),
    m_isComposite(true),
    m_isContainer(false)
    {
         m_size =  _size();
        size_t offset = 0;
        for(auto & member : m_members){
            member.offset = offset;
            offset+=member.type.footprint();
        }
    }

    Type::Type(std::string name, Type t):m_name(name),
    m_size(t.footprint()),
    m_members({{"ContainedTypeInfo",t}}),
    m_isPrimitve(false),
    m_isComposite(false),
    m_isContainer(true)
    {

    }


    bool Type::isPrimitive() const {
        return m_isPrimitve;
    }

    size_t Type::footprint() const {
            return m_size;
    }

    const vector<Type::Member>
    Type::getMembers() const {

        return m_members;
    }

    size_t Type::sizeOf(wlo::data::Type type) {
       return  type.footprint();
    }

    bool Type::operator==(const Type &other) const {
        if(footprint()!=other.footprint())
            return false;
        if(getMembers().size() != other.getMembers().size())
            return false;
        if(isPrimitive()&&other.isPrimitive()){
            if(m_name!=other.name())
                return false;
        }
        for(size_t i = 0; i< getMembers().size(); i++) {
            if (getMembers()[i].name != other.getMembers()[i].name)
                return false;
            if (getMembers()[i].type != other.getMembers()[i].type)
                return false;
        }
        return true;
    }

    const std::string Type::name() const {
      return m_name;
    }

    bool Type::compatibleWith(const Type &other) const {
        if(footprint()!=other.footprint() )
            return false;
        if(getMembers().size() != other.getMembers().size())
            return false;
        for(size_t i = 0; i< getMembers().size(); i++) {
            auto member = getMembers()[i];
            auto otherMember = other.getMembers()[i];
            if(member.type.isPrimitive()&&otherMember.type.isPrimitive() &&(member.type.name()!=otherMember.type.name()) )
                        return false;
            else
                if(!member.type.compatibleWith(otherMember.type))
                    return false;
        }

        return true;
    }

    size_t Type::_size() const {
        if(isPrimitive())//for primitive types, floats, ints, etc just return the size
            return m_size;

        size_t size = 0;
        for(const auto & member : m_members)//for composite types, return the sum of member sizes
            size+=member.type._size();
        return size;
    }

    bool Type::isComposite() const {
        return !m_isPrimitve;
    }


    bool Type::isContainer() const {
        return false;
    }


}
