//
// Created by W on 12/5/20.
//

#ifndef WILLOW_TYPE_HPP
#define WILLOW_TYPE_HPP
#include "willow/root/Root.hpp"
    namespace wlo::data {

        class Type{
        public:
            struct Member{
                std::string name;
                const Type&  type;
                size_t offset;
            };
            Type() noexcept;
            Type(std::string name, size_t size);
            Type(std::string name, std::vector<Member> members);
            Type & operator =(const Type & other){
                m_name = other.m_name;
                m_members.clear();
                for (auto & member : other.m_members )
                    m_members.push_back(member);
                m_size = _size();
                m_isPrimitve = other.isPrimitive();
                m_isComposite = other.isComposite();
                m_isContainer = other.isContainer();
                return *this;
            }
//            Type(std::string name, const Type & t);


            //two types are compatible if their memory sizes, and the types of all of their fields are the same,
            //but have different member names
            //asks the question, can I de-serialize this data into a struct with Type::of<T>() without faulting?
            bool compatibleWith(const Type & other) const;

            //two types are equal only if they have all of the field same names, and field types
            //do these types represent the same primitive type? or
            //do these types represent the same struct?
            bool operator==(const Type &other) const ;
            [[nodiscard]] bool isPrimitive()const;
            [[nodiscard]] bool isComposite()const;
            [[nodiscard]] bool isContainer()const;
            [[nodiscard]] size_t footprint() const ;
            [[nodiscard]] const std::string name() const;
            [[nodiscard]] const std::vector<Member>  getMembers() const;




        protected:
            size_t _size() const;
            std::vector<Member> m_members;
            std::string m_name;
            bool m_isPrimitve;
            bool m_isContainer;
            bool m_isComposite;
            size_t m_size;
        };




    }



inline std::ostream& operator<<(std::ostream & o, const wlo::data::Type&  type){
    o<<"< Data: totalMemorySize: "<<type.footprint()<<std::endl;
    for(auto [name,type, offset] : type.getMembers())
        o<<type<<", offset: "<<offset<<std::endl;
    o<<">";
    return o;
}

namespace std {
  template <>
  struct hash<wlo::data::Type>
  {
    std::size_t operator()(const wlo::data::Type& layout) const
    {
      using std::size_t;
      using std::hash;
      using std::string;
      using wlo::data::Type;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      if(layout.isPrimitive())
          return hash<string>()(layout.name())^((hash<size_t>()(layout.footprint())));

      size_t hash_value = hash<string>()(layout.getMembers()[0].name)^((hash<wlo::data::Type>()(layout.getMembers()[0].type)<<1)>>1) ^((layout.getMembers()[0].offset << 2) >> 2);
          for (size_t i = 1; i < layout.getMembers().size(); i++)
              hash_value ^= hash<string>()(layout.getMembers()[i].name)^((hash<wlo::data::Type>()(layout.getMembers()[i].type)<<(i+1))>>(i+1)) ^ ((layout.getMembers()[i].offset << (i + 2)) >> (i + 2));

    return hash_value;
    }
  };

  template <>
  struct equal_to<wlo::data::Type>
  {
    bool operator()(const wlo::data::Type& one, const wlo::data::Type& two) const
    {
      using namespace wlo;
     if(one.footprint()!=two.footprint() )
         return false;
     if(one.getMembers().size() != two.getMembers().size())
         return false;
    for(size_t i = 0; i< one.getMembers().size(); i++)
        if(one.getMembers()[i].type != two.getMembers()[i].type)
            return false;
   return true;

    }
  };
}

#include"TypeOf.hpp"




#endif //WILLOW_TYPE_HPP
